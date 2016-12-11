#include "TransformWidget.hpp"

TransformWidget::TransformWidget(QWidget *parent)
	: InspectorSubWidget(parent)
	, m_transXEdit(shared_ptr<QLineEdit>(new QLineEdit("0")))
	, m_transYEdit(shared_ptr<QLineEdit>(new QLineEdit("0")))
	, m_transZEdit(shared_ptr<QLineEdit>(new QLineEdit("0")))
	, m_resetRotate(shared_ptr<QPushButton>(new QPushButton("Reset")))
	, m_scaleXEdit(shared_ptr<QLineEdit>(new QLineEdit("1")))
	, m_scaleYEdit(shared_ptr<QLineEdit>(new QLineEdit("1")))
	, m_scaleZEdit(shared_ptr<QLineEdit>(new QLineEdit("1")))
	, m_lastTranslate(glm::vec3(0))
	, m_lastScale(glm::vec3(1.0f))
{
	QHBoxLayout *mainLayout = new QHBoxLayout;
		QVBoxLayout *leftLayout = new QVBoxLayout;
		QVBoxLayout *rightLayout = new QVBoxLayout;
			QHBoxLayout *translateLayout = new QHBoxLayout;
			QHBoxLayout *rotateLayout = new QHBoxLayout;
			QHBoxLayout *scaleLayout = new QHBoxLayout;

	m_transXEdit->setFixedWidth(70);
	m_transYEdit->setFixedWidth(70);
	m_transZEdit->setFixedWidth(70);
	m_scaleXEdit->setFixedWidth(70);
	m_scaleYEdit->setFixedWidth(70);
	m_scaleZEdit->setFixedWidth(70);

	QLabel *lbl;

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);

	leftLayout->addWidget(new QLabel("Position"));
	leftLayout->addWidget(new QLabel("Rotate"));
	leftLayout->addWidget(new QLabel("Scale"));

	rightLayout->addLayout(translateLayout);
	rightLayout->addLayout(rotateLayout);
	rightLayout->addLayout(scaleLayout);
	
	// Position
	lbl = new QLabel("X");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	translateLayout->addWidget(lbl);
	translateLayout->addWidget(m_transXEdit.get());
	lbl = new QLabel("Y");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	translateLayout->addWidget(lbl);
	translateLayout->addWidget(m_transYEdit.get());
	lbl = new QLabel("Z");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	translateLayout->addWidget(lbl);
	translateLayout->addWidget(m_transZEdit.get());

	// Rotate
	m_resetRotate->setFixedHeight(30);
	m_resetRotate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	//rotateLayout->addStretch();
	rotateLayout->addWidget(m_resetRotate.get());

	// Scale
	lbl = new QLabel("X");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	scaleLayout->addWidget(lbl);
	scaleLayout->addWidget(m_scaleXEdit.get());
	lbl = new QLabel("Y");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	scaleLayout->addWidget(lbl);
	scaleLayout->addWidget(m_scaleYEdit.get());
	lbl = new QLabel("Z");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	scaleLayout->addWidget(lbl);
	scaleLayout->addWidget(m_scaleZEdit.get());

	lbl = new QLabel("Transform");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(mainLayout);

	connect(m_transXEdit.get(), &QLineEdit::textEdited, this, &TransformWidget::_onTransformChange);
	connect(m_transYEdit.get(), &QLineEdit::textEdited, this, &TransformWidget::_onTransformChange);
	connect(m_transZEdit.get(), &QLineEdit::textEdited, this, &TransformWidget::_onTransformChange);
	connect(m_scaleXEdit.get(), &QLineEdit::textEdited, this, &TransformWidget::_onTransformChange);
	connect(m_scaleYEdit.get(), &QLineEdit::textEdited, this, &TransformWidget::_onTransformChange);
	connect(m_scaleZEdit.get(), &QLineEdit::textEdited, this, &TransformWidget::_onTransformChange);
	connect(m_resetRotate.get(), &QPushButton::clicked, this, &TransformWidget::_onRotateReset);
}

void TransformWidget::_onTransformChange(QString newValue)
{
	boost::lock_guard<TransformWidget> guard(*this);
	if (m_currentNode && m_currentNode->getObject() && m_currentNode->getObject()->getTransformComponent())
	{
		shared_ptr<TransformComponent> transform = m_currentNode->getObject()->getTransformComponent();
		transform->removeChangeListener(this);
		transform->setTranslate(glm::vec3(
			m_transXEdit->text().toFloat(),
			m_transYEdit->text().toFloat(),
			m_transZEdit->text().toFloat()
		));
		transform->setScale(glm::vec3(
			m_scaleXEdit->text().toFloat(),
			m_scaleYEdit->text().toFloat(),
			m_scaleZEdit->text().toFloat()
		));
		transform->addChangeListener(this, [this](auto transform) {this->_refresh(transform);});
	}
}

void TransformWidget::_onRotateReset()
{
	boost::lock_guard<TransformWidget> guard(*this);
	if (m_currentNode && m_currentNode->getObject() && m_currentNode->getObject()->getTransformComponent())
	{
		shared_ptr<TransformComponent> transform = m_currentNode->getObject()->getTransformComponent();
		transform->setRotate(glm::mat4(1.0));
	}
}

void TransformWidget::_refresh(SceneNode *newNode)
{
	boost::lock_guard<TransformWidget> guard(*this);
	if (!newNode->getObject() || !newNode->getObject()->getTransformComponent())
	{
		this->hide();
		return;
	}

	shared_ptr<TransformComponent> transform = newNode->getObject()->getTransformComponent();
	_refresh(transform.get());

	if (m_lastActiveNode && m_lastActiveNode.get() != newNode)
	{
		if (m_lastActiveNode->getObject() && m_lastActiveNode->getObject()->getTransformComponent())
		{
			shared_ptr<TransformComponent> lastTransform = m_lastActiveNode->getObject()->getTransformComponent();
			lastTransform->removeChangeListener(this);
		}
	}

	if (!m_lastActiveNode || m_lastActiveNode.get() != newNode)
	{
		transform->addChangeListener(this, [this](auto transform) {this->_refresh(transform);});
	}
}

void TransformWidget::_refresh(TransformComponent *transform)
{
	if (m_currentNode)
	{
		glm::vec3 translate = *m_currentNode->getObject()->getTransformComponent()->getTranslate();
		glm::vec3 scale = *m_currentNode->getObject()->getTransformComponent()->getScale();
		if (translate != m_lastTranslate || scale != m_lastScale)
		{
			boost::lock_guard<boost::mutex> guard(m_textLock);
			m_transXEdit->setText(QString::number(translate.x).left(7));
			m_transYEdit->setText(QString::number(translate.y).left(7));
			m_transZEdit->setText(QString::number(translate.z).left(7));
			m_scaleXEdit->setText(QString::number(scale.x).left(7));
			m_scaleYEdit->setText(QString::number(scale.y).left(7));
			m_scaleZEdit->setText(QString::number(scale.z).left(5));
			m_lastTranslate = translate;
			m_lastScale = scale;
		}
	}
}