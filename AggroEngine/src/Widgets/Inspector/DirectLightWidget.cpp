#include "DirectLightWidget.hpp"

DirectLightWidget::DirectLightWidget(QWidget *parent)
	: InspectorSubWidget(parent)
	, m_ambientSlider(new QSlider(Qt::Horizontal))
	, m_colorREdit(new QLineEdit("1.0"))
	, m_colorGEdit(new QLineEdit("1.0"))
	, m_colorBEdit(new QLineEdit("1.0"))
{
	QHBoxLayout *mainLayout = new QHBoxLayout;
		QVBoxLayout *leftLayout = new QVBoxLayout;
		QVBoxLayout *rightLayout = new QVBoxLayout;
			QHBoxLayout *ambientLayout = new QHBoxLayout;
			QHBoxLayout *colorLayout = new QHBoxLayout;

	m_colorREdit->setFixedWidth(70);
	m_colorGEdit->setFixedWidth(70);
	m_colorBEdit->setFixedWidth(70);

	QLabel *lbl;

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);

	leftLayout->addWidget(new QLabel("Ambient"));
	leftLayout->addWidget(new QLabel("Color"));

	rightLayout->addLayout(ambientLayout);
	rightLayout->addLayout(colorLayout);

	// Ambient
	ambientLayout->addWidget(m_ambientSlider.get());

	// Color
	lbl = new QLabel("R");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	colorLayout->addWidget(lbl);
	colorLayout->addWidget(m_colorREdit.get());
	lbl = new QLabel("G");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	colorLayout->addWidget(lbl);
	colorLayout->addWidget(m_colorGEdit.get());
	lbl = new QLabel("B");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	colorLayout->addWidget(lbl);
	colorLayout->addWidget(m_colorBEdit.get());

	lbl = new QLabel("Direct Light");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(mainLayout);

	connect(m_ambientSlider.get(), &QSlider::valueChanged, this, &DirectLightWidget::_onAmbientChange);
	connect(m_colorREdit.get(), &QLineEdit::textEdited, this, &DirectLightWidget::_onColorChange);
	connect(m_colorGEdit.get(), &QLineEdit::textEdited, this, &DirectLightWidget::_onColorChange);
	connect(m_colorBEdit.get(), &QLineEdit::textEdited, this, &DirectLightWidget::_onColorChange);
}

void DirectLightWidget::_onAmbientChange(int value)
{
	boost::lock_guard<DirectLightWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasDirectLightComponent())
	{
		shared_ptr<DirectLightComponent> light = m_currentNode->getDirectLightComponent();
		light->removeChangeListener(this);
		light->setAmbientPct(value);
		light->addChangeListener(this, [this](auto newLight) {this->_refresh(newLight); });
	}
}

void DirectLightWidget::_onColorChange(QString newValue)
{
	boost::lock_guard<DirectLightWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasDirectLightComponent())
	{
		shared_ptr<DirectLightComponent> light = m_currentNode->getDirectLightComponent();
		light->removeChangeListener(this);
		light->setColor(glm::vec3(
			m_colorREdit->text().toFloat(),
			m_colorGEdit->text().toFloat(),
			m_colorBEdit->text().toFloat()
		));
		light->addChangeListener(this, [this](auto newLight) {this->_refresh(newLight); });
	}
}

void DirectLightWidget::_refresh(SceneNode *newNode)
{
	boost::lock_guard<DirectLightWidget> guard(*this);
	if (!newNode->hasDirectLightComponent())
	{
		this->hide();
		return;
	}

	shared_ptr<DirectLightComponent> light = newNode->getDirectLightComponent();
	_refresh(light.get());

	if (m_lastActiveNode && m_lastActiveNode.get() != newNode)
	{
		if (m_lastActiveNode->hasDirectLightComponent())
		{
			shared_ptr<DirectLightComponent> lastLight = m_lastActiveNode->getDirectLightComponent();
			lastLight->removeChangeListener(this);
		}
	}

	if (!m_lastActiveNode || m_lastActiveNode.get() != newNode)
	{
		light->addChangeListener(this, [this](auto newLight) {this->_refresh(newLight); });
	}
}

void DirectLightWidget::_refresh(DirectLightComponent *directLight)
{
	if (m_currentNode)
	{
		glm::vec3 color = directLight->getColor();
		int ambient = directLight->getAmbientPct();
		if (color != m_lastColor && m_lastAmbient != ambient)
		{
			boost::lock_guard<boost::mutex> guard(m_textLock);
			static const QRegExp trailingZeros("0+$");
			static const QRegExp trailingDot("\\.$");
			m_colorREdit->setText(QString::number(color.x, 'f', 3).remove(trailingZeros).remove(trailingDot));
			m_colorGEdit->setText(QString::number(color.y, 'f', 3).remove(trailingZeros).remove(trailingDot));
			m_colorBEdit->setText(QString::number(color.z, 'f', 3).remove(trailingZeros).remove(trailingDot));
			m_lastColor = color;
			m_ambientSlider->setValue(ambient);
			m_lastAmbient = ambient;
		}
	}
}