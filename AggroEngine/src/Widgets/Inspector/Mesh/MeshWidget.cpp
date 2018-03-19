#include "MeshWidget.hpp"
#include "FileBackedMesh.hpp"
#include <QDir>
#include <QFileDialog>

MeshWidget::MeshWidget(QWidget *parent, shared_ptr<EngineContext> context)
	: m_meshEdit(new QLineEdit(""))
	, m_triangleCountLbl(new QLabel(""))
	, m_alignCombo(new QComboBox())
	, m_normalLineChk(new QCheckBox("enabled"))
	, m_resources(context->getResources())
	, m_meshCache(context->getMeshCache())
	, m_boundingBoxChk(new QCheckBox("enabled"))
{
	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *leftLayout = new QVBoxLayout;
	QVBoxLayout *rightLayout = new QVBoxLayout;
		QHBoxLayout *sourceLayout = new QHBoxLayout;
		QHBoxLayout *trianglesLayout = new QHBoxLayout;
		QHBoxLayout *alignLayout = new QHBoxLayout;
		QHBoxLayout *normalLinesLayout = new QHBoxLayout;
		QHBoxLayout *boundingBoxLayout = new QHBoxLayout;

	QLabel *lbl;

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);

	leftLayout->addWidget(new QLabel("Mesh"));
	leftLayout->addWidget(new QLabel("Triangles:"));
	leftLayout->addWidget(new QLabel("Align:"));
	leftLayout->addWidget(new QLabel("Normal Lines:"));
	leftLayout->addWidget(new QLabel("Bounding Box:"));

	rightLayout->addLayout(sourceLayout);
	rightLayout->addLayout(trianglesLayout);
	rightLayout->addLayout(alignLayout);
	rightLayout->addLayout(normalLinesLayout);
	rightLayout->addLayout(boundingBoxLayout);

	QPushButton *selectMeshButton = new QPushButton("...");
	selectMeshButton->setFixedWidth(50);
	selectMeshButton->setFixedHeight(25);
	sourceLayout->addWidget(m_meshEdit.get());
	sourceLayout->addWidget(selectMeshButton);

	trianglesLayout->addWidget(m_triangleCountLbl.get());
	trianglesLayout->addStretch();

	m_alignCombo->addItem("None");
	m_alignCombo->addItem("Center");
	m_alignCombo->addItem("Bottom");
	alignLayout->addWidget(m_alignCombo.get());
	
	m_normalLineChk->setStyleSheet(QString("border-bottom-style: none;"));
	normalLinesLayout->addWidget(m_normalLineChk.get());

	m_boundingBoxChk->setStyleSheet(QString("border-bottom-style: none;"));
	boundingBoxLayout->addWidget(m_boundingBoxChk.get());

	lbl = new QLabel("Mesh");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(mainLayout);

	connect(selectMeshButton, &QPushButton::pressed, this, &MeshWidget::_onMeshSelect);
	connect(m_normalLineChk.get(), &QCheckBox::stateChanged, this, &MeshWidget::_onNormalLineCheck);
	connect(m_boundingBoxChk.get(), &QCheckBox::stateChanged, this, &MeshWidget::_onBoundingBoxCheck);
	connect(m_alignCombo.get(), QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int idx) {
		if (m_currentNode && m_currentNode->hasMeshComponent())
		{
			m_currentNode->getMeshComponent()->setAxisAlign((MeshComponent::AxisAlign)idx);
		}
	});
}

void MeshWidget::_refresh(SceneNode *newNode)
{
	boost::lock_guard<MeshWidget> guard(*this);
	if (!newNode->hasMeshComponent() || newNode->hasDirectLightComponent() || newNode->hasCameraComponent())
	{
		this->hide();
		return;
	}

	shared_ptr<MeshComponent> mesh = newNode->getMeshComponent();
	_refresh(mesh.get());

	if (m_lastActiveNode && m_lastActiveNode.get() != newNode)
	{
		if (m_lastActiveNode->hasMeshComponent())
		{
			shared_ptr<MeshComponent> material = m_lastActiveNode->getMeshComponent();
			mesh->removeChangeListener(this);
		}
	}

	if (!m_lastActiveNode || m_lastActiveNode.get() != newNode)
	{
		mesh->addChangeListener(this, [this](auto newMesh) {this->_refresh(newMesh); });
	}
}

void MeshWidget::_refresh(MeshComponent *mesh)
{
	m_alignCombo->blockSignals(true);
	m_normalLineChk->blockSignals(true);
	m_boundingBoxChk->blockSignals(true);
	std::shared_ptr<Mesh> primaryMesh = mesh->getPrimaryMesh();

	if (primaryMesh)
	{
		string filename = primaryMesh->getMeshPath().get_value_or("");
		m_meshEdit->setText(QString::fromStdString(filename));
		m_triangleCountLbl->setText(QString::number(primaryMesh->getSizeOfIndicies() / 12));
	}
	else
	{
		m_meshEdit->setText("");
	}

	m_normalLineChk->setChecked(mesh->isNormalLinesEnabled());
	m_boundingBoxChk->setChecked(mesh->isBoundingBoxEnabled());
	m_alignCombo->setCurrentIndex(mesh->getAxisAlign());
	m_alignCombo->blockSignals(false);
	m_normalLineChk->blockSignals(false);
	m_boundingBoxChk->blockSignals(false);
}

void MeshWidget::_onMeshSelect()
{
	boost::lock_guard<MeshWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMeshComponent())
	{
		QDir workingDirectory = QDir::current();
		QString filename = QFileDialog::getOpenFileName(this, tr("Select Mesh"), workingDirectory.path() + "/Resources/Mesh/", "*");
		filename = workingDirectory.relativeFilePath(filename);
		m_meshEdit->setText(filename);

		shared_ptr<MeshComponent> mesh = m_currentNode->getMeshComponent();
		mesh->removeChangeListener(this);
		if (!filename.isEmpty())
		{
			mesh->clearMeshes();
			int id = m_resources->getIdForPath(filename.toStdString());
			m_meshCache->getMesh(id)->onReady([this, mesh](auto newMesh) {mesh->setPrimaryMesh(newMesh); });
			_refresh(mesh.get());
		}
		mesh->addChangeListener(this, [this](auto newMesh) {this->_refresh(newMesh); });
	}
}

void MeshWidget::_onNormalLineCheck(int state)
{
	if (m_currentNode && m_currentNode->hasMeshComponent())
	{
		m_currentNode->getMeshComponent()->enableNormalLines(state == Qt::Checked, state == Qt::Checked);
	}
}

void MeshWidget::_onBoundingBoxCheck(int state)
{
	if (m_currentNode && m_currentNode->hasMeshComponent())
	{
		m_currentNode->getMeshComponent()->enableBoundingBox(state == Qt::Checked, state == Qt::Checked);
	}
}