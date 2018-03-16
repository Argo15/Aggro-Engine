#include "CameraWidget.hpp"

CameraWidget::CameraWidget(InspectorWidget *parent, shared_ptr<EngineContext> context)
	: InspectorSubWidget(parent)
	, m_setCurrent(new QPushButton("Set Current"))
    , m_leaveCamera(new QPushButton("Leave Camera"))
{
	QHBoxLayout *mainLayout = new QHBoxLayout;

	QLabel *lbl;

	mainLayout->addWidget(m_setCurrent.get());
	mainLayout->addWidget(m_leaveCamera.get());

	lbl = new QLabel("Camera");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(mainLayout);

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
}