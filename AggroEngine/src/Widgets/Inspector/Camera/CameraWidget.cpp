#include "CameraWidget.hpp"

CameraWidget::CameraWidget(InspectorWidget *parent, shared_ptr<EngineContext> context)
	: InspectorSubWidget(parent)
	, m_setCurrent(new QPushButton("Set Current"))
    , m_leaveCamera(new QPushButton("Leave Camera"))
	, m_overrideFrustrumChk(new QCheckBox("enabled"))
{
	QHBoxLayout *buttonLayout = new QHBoxLayout;
	QHBoxLayout *lrLayout = new QHBoxLayout;
		QVBoxLayout *leftLayout = new QVBoxLayout;
		QVBoxLayout *rightLayout = new QVBoxLayout;

	QLabel *lbl;

	buttonLayout->addWidget(m_setCurrent.get());
	buttonLayout->addWidget(m_leaveCamera.get());
	lrLayout->addLayout(leftLayout);
	lrLayout->addLayout(rightLayout);

	leftLayout->addWidget(new QLabel("Frustrum Override:"));

	m_overrideFrustrumChk->setStyleSheet(QString("border-bottom-style: none;"));
	rightLayout->addWidget(m_overrideFrustrumChk.get());

	lbl = new QLabel("Camera");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(buttonLayout);
	m_layout->addLayout(lrLayout);

	connect(m_setCurrent.get(), &QPushButton::pressed, this, [this, context]() {
		if (m_currentNode)
		{
			context->getScene()->setCamera(m_currentNode);
		}
	});
	connect(m_leaveCamera.get(), &QPushButton::pressed, this, [this, context]() {
		if (m_currentNode)
		{
			shared_ptr<SceneNode> newCamera(new SceneNode(Scene::getNextId()));
			newCamera->setCameraComponent(shared_ptr<CameraComponent>(
				new CameraComponent(m_currentNode->getCameraComponent(), context->getScene().get())));
			newCamera->setTransformComponent(shared_ptr<TransformComponent>(
				new TransformComponent(m_currentNode->getTransformComponent())));
			context->getScene()->setCamera(newCamera);
		}
	});
	connect(m_overrideFrustrumChk.get(), &QCheckBox::stateChanged, this, [this](auto state) {
		if (m_currentNode && m_currentNode->hasCameraComponent())
		{
			m_currentNode->getCameraComponent()->setOverrideFrustrum(state == Qt::Checked);
		}
	});

}

void CameraWidget::_refresh(SceneNode *newNode)
{
	boost::lock_guard<CameraWidget> guard(*this);
	if (!newNode->hasCameraComponent())
	{
		this->hide();
		return;
	}

	shared_ptr<CameraComponent> camera = newNode->getCameraComponent();
	_refresh(camera.get());

	if (m_lastActiveNode && m_lastActiveNode.get() != newNode)
	{
		if (m_lastActiveNode->hasCameraComponent())
		{
			shared_ptr<CameraComponent> lastCamera = m_lastActiveNode->getCameraComponent();
			lastCamera->removeChangeListener(this);
		}
	}

	if (!m_lastActiveNode || m_lastActiveNode.get() != newNode)
	{
		camera->addChangeListener(this, [this](auto newCamera) {this->_refresh(newCamera); });
	}

}

void CameraWidget::_refresh(CameraComponent *camera)
{
	m_overrideFrustrumChk->blockSignals(true);
	if (camera->isActive())
	{
		m_setCurrent->setDisabled(true);
		m_leaveCamera->setEnabled(true);
	}
	else
	{
		m_setCurrent->setEnabled(true);
		m_leaveCamera->setDisabled(true);
	}

	m_overrideFrustrumChk->setChecked(camera->shouldOverrideFrustrum());
	m_overrideFrustrumChk->blockSignals(false);
}