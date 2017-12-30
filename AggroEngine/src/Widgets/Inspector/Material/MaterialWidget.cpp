#include "MaterialWidget.hpp"
#include <QDir>
#include <QFileDialog>

MaterialWidget::MaterialWidget(QWidget *parent, shared_ptr<Resources> resources)
	: InspectorSubWidget(parent)
	, m_colorREdit(new QLineEdit("0"))
	, m_colorGEdit(new QLineEdit("0"))
	, m_colorBEdit(new QLineEdit("0"))
	, m_textureEdit(new QLineEdit(""))
	, m_texScaleUEdit(new QLineEdit(""))
	, m_texScaleVEdit(new QLineEdit(""))
	, m_texOffsetUEdit(new QLineEdit(""))
	, m_texOffsetVEdit(new QLineEdit(""))
	, m_texRotateSlider(new QSlider(Qt::Horizontal))
	, m_alphaEdit(new QLineEdit(""))
	, m_specIntensitySlider(new QSlider(Qt::Horizontal))
	, m_specShineSlider(new QSlider(Qt::Horizontal))
	, m_specMapEdit(new QLineEdit(""))
	, m_resources(resources)
{
	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *leftLayout = new QVBoxLayout;
	QVBoxLayout *rightLayout = new QVBoxLayout;
		QHBoxLayout *colorLayout = new QHBoxLayout;
		QHBoxLayout *textureLayout = new QHBoxLayout;
		QHBoxLayout *alphaLayout = new QHBoxLayout;
		QHBoxLayout *specIntensityLayout = new QHBoxLayout;
		QHBoxLayout *specShineLayout = new QHBoxLayout;
		QHBoxLayout *specMapLayout = new QHBoxLayout;
		QHBoxLayout *texScaleLayout = new QHBoxLayout;
		QHBoxLayout *texOffsetLayout = new QHBoxLayout;
		QHBoxLayout *texRotateLayout = new QHBoxLayout;

	m_colorREdit->setFixedWidth(70);
	m_colorGEdit->setFixedWidth(70);
	m_colorBEdit->setFixedWidth(70);
	m_texScaleUEdit->setFixedWidth(70);
	m_texScaleVEdit->setFixedWidth(70);
	m_texOffsetUEdit->setFixedWidth(70);
	m_texOffsetVEdit->setFixedWidth(70);

	QLabel *lbl;

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);

	leftLayout->addWidget(new QLabel("Color"));
	leftLayout->addWidget(new QLabel("Texture"));
	leftLayout->addWidget(new QLabel("Tex Scale"));
	leftLayout->addWidget(new QLabel("Tex Offset"));
	leftLayout->addWidget(new QLabel("Tex Rotate"));
	leftLayout->addWidget(new QLabel("Alpha"));
	leftLayout->addWidget(new QLabel("Spec Intensity"));
	leftLayout->addWidget(new QLabel("Spec Shininess"));
	leftLayout->addWidget(new QLabel("Spec Map"));

	rightLayout->addLayout(colorLayout);
	rightLayout->addLayout(textureLayout);
	rightLayout->addLayout(texScaleLayout);
	rightLayout->addLayout(texOffsetLayout);
	rightLayout->addLayout(texRotateLayout);
	rightLayout->addLayout(alphaLayout);
	rightLayout->addLayout(specIntensityLayout);
	rightLayout->addLayout(specShineLayout);
	rightLayout->addLayout(specMapLayout);

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

	QPushButton *selectTexButton = new QPushButton("...");
	selectTexButton->setFixedWidth(50);
	selectTexButton->setFixedHeight(25);
	textureLayout->addWidget(m_textureEdit.get());
	textureLayout->addWidget(selectTexButton);

	// Tex Scale
	lbl = new QLabel("U");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	texScaleLayout->addWidget(lbl);
	texScaleLayout->addWidget(m_texScaleUEdit.get());
	lbl = new QLabel("V");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	texScaleLayout->addWidget(lbl);
	texScaleLayout->addWidget(m_texScaleVEdit.get());
	texScaleLayout->addStretch();
	texScaleLayout->addStretch();

	// Tex Offset
	lbl = new QLabel("U");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	texOffsetLayout->addWidget(lbl);
	texOffsetLayout->addWidget(m_texOffsetUEdit.get());
	lbl = new QLabel("V");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	texOffsetLayout->addWidget(lbl);
	texOffsetLayout->addWidget(m_texOffsetVEdit.get());
	texOffsetLayout->addStretch();
	texOffsetLayout->addStretch();

	m_texRotateSlider->setStyleSheet(QString("border-bottom-style: none;"));
	texRotateLayout->addWidget(m_texRotateSlider.get());

	QPushButton *selectAlphaButton = new QPushButton("...");
	selectAlphaButton->setFixedWidth(50);
	selectAlphaButton->setFixedHeight(25);
	alphaLayout->addWidget(m_alphaEdit.get());
	alphaLayout->addWidget(selectAlphaButton);

	m_specIntensitySlider->setStyleSheet(QString("border-bottom-style: none;"));
	specIntensityLayout->addWidget(m_specIntensitySlider.get());
	m_specShineSlider->setMinimum(5);
	m_specShineSlider->setMaximum(100);
	m_specShineSlider->setStyleSheet(QString("border-bottom-style: none;"));
	specShineLayout->addWidget(m_specShineSlider.get());
	QPushButton *selectSpecButton = new QPushButton("...");
	selectSpecButton->setFixedWidth(50);
	selectSpecButton->setFixedHeight(25);
	specMapLayout->addWidget(m_specMapEdit.get());
	specMapLayout->addWidget(selectSpecButton);

	lbl = new QLabel("Material");
	lbl->setStyleSheet("font-weight: bold; font-size: 16px;");
	m_layout->addWidget(lbl);
	m_layout->addLayout(mainLayout);

	connect(m_colorREdit.get(), &QLineEdit::textEdited, this, &MaterialWidget::_onColorChange);
	connect(m_colorGEdit.get(), &QLineEdit::textEdited, this, &MaterialWidget::_onColorChange);
	connect(m_colorBEdit.get(), &QLineEdit::textEdited, this, &MaterialWidget::_onColorChange);
	connect(selectTexButton, &QPushButton::pressed, this, [this]() {
		this->_onTexSelect(m_textureEdit,
			[this](auto material, int id) { material->setTextureImageId(id); },
			[this](auto material) { material->removeTexture(); }
		);
	});
	connect(m_texScaleUEdit.get(), &QLineEdit::textEdited, this, [this](QString value) {
		_onValueSelect(value, [this](auto material, float val) { material->setTexScaleU(val); });
	});
	connect(m_texScaleVEdit.get(), &QLineEdit::textEdited, this, [this](QString value) {
		_onValueSelect(value, [this](auto material, float val) { material->setTexScaleV(val); });
	});
	connect(m_texOffsetUEdit.get(), &QLineEdit::textEdited, this, [this](QString value) {
		_onValueSelect(value, [this](auto material, float val) { material->setTexOffsetU(val); });
	});
	connect(m_texOffsetVEdit.get(), &QLineEdit::textEdited, this, [this](QString value) {
		_onValueSelect(value, [this](auto material, float val) { material->setTexOffsetV(val); });
	});
	connect(m_texRotateSlider.get(), &QSlider::valueChanged, this, [this](int value) {
		_onSliderChange(value, [this](auto material, int val) { material->setTexRotate((2 * PI) * val / 100.0f); });
	});
	connect(selectAlphaButton, &QPushButton::pressed, this, [this]() {
		this->_onTexSelect(m_alphaEdit,
			[this](auto material, int id) { material->setAlphaImageId(id); },
			[this](auto material) { material->removeAlphaMap(); }
		);
	});
	connect(m_specIntensitySlider.get(), &QSlider::valueChanged, this, [this](int value) {
		_onSliderChange(value, [this](auto material, int val) { material->setSpecIntensityPct(val); });
	});
	connect(m_specShineSlider.get(), &QSlider::valueChanged, this, [this](int value) {
		_onSliderChange(value, [this](auto material, int val) { material->setShininess(val); });
	});
	connect(selectSpecButton, &QPushButton::pressed, this, [this]() {
		this->_onTexSelect(m_specMapEdit,
			[this](auto material, int id) { material->setSpecularImageId(id); },
			[this](auto material) { material->removeSpecularMap(); }
		);
	});
}

void MaterialWidget::_onColorChange(QString newValue)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMaterialComponent())
	{
		shared_ptr<MaterialComponent> material = m_currentNode->getMaterialComponent();
		material->removeChangeListener(this);
		material->setColor(glm::vec3(
			m_colorREdit->text().toFloat(),
			m_colorGEdit->text().toFloat(),
			m_colorBEdit->text().toFloat()
		));
		material->addChangeListener(this, [this](auto newMaterial) {this->_refresh(newMaterial); });
	}
}

void MaterialWidget::_onValueSelect(QString newValue, function<void(shared_ptr<MaterialComponent>, float)> acceptFunc)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMaterialComponent())
	{
		shared_ptr<MaterialComponent> material = m_currentNode->getMaterialComponent();
		material->removeChangeListener(this);
		acceptFunc(material, newValue.toFloat());
		material->addChangeListener(this, [this](auto newMaterial) {this->_refresh(newMaterial); });
	}
}

void MaterialWidget::_onTexSelect(shared_ptr<QLineEdit> textureEdit,
								  function<void(shared_ptr<MaterialComponent>, int)> setImageFunc, 
								  function<void(shared_ptr<MaterialComponent>)> removeFunc)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMaterialComponent())
	{
		QDir workingDirectory = QDir::current();
		QString filename = QFileDialog::getOpenFileName(this, tr("Select Texture"), workingDirectory.path() + "/Resources/Textures/", "*");
		filename = workingDirectory.relativeFilePath(filename);
		textureEdit->setText(filename);

		shared_ptr<MaterialComponent> material = m_currentNode->getMaterialComponent();
		material->removeChangeListener(this);
		if (!filename.isEmpty())
		{
			setImageFunc(material, m_resources->getIdForPath(filename.toStdString()));
		}
		else
		{
			removeFunc(material);
		}
		material->addChangeListener(this, [this](auto newMaterial) {this->_refresh(newMaterial); });
	}
}

void MaterialWidget::_onSliderChange(int value, function<void(shared_ptr<MaterialComponent>, int)> acceptFunc)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMaterialComponent())
	{
		shared_ptr<MaterialComponent> material = m_currentNode->getMaterialComponent();
		material->removeChangeListener(this);
		acceptFunc(material, value);
		material->addChangeListener(this, [this](auto newMaterial) {this->_refresh(newMaterial); });
	}
}

void MaterialWidget::_onSpecularChange(int value)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMaterialComponent())
	{
		shared_ptr<MaterialComponent> material = m_currentNode->getMaterialComponent();
		material->removeChangeListener(this);
		material->setSpecIntensityPct(value);
		material->addChangeListener(this, [this](auto newMaterial) {this->_refresh(newMaterial); });
	}
}

void MaterialWidget::_onShininessChange(int value)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMaterialComponent())
	{
		shared_ptr<MaterialComponent> material = m_currentNode->getMaterialComponent();
		material->removeChangeListener(this);
		material->setShininess(value);
		material->addChangeListener(this, [this](auto newMaterial) {this->_refresh(newMaterial); });
	}
}

void MaterialWidget::_refresh(SceneNode *newNode)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (!newNode->hasMaterialComponent() || 
		newNode->hasDirectLightComponent() || 
		newNode->getMaterialComponent()->getOwner() != newNode)
	{
		this->hide();
		return;
	}

	shared_ptr<MaterialComponent> material = newNode->getMaterialComponent();
	_refresh(material.get());

	if (m_lastActiveNode && m_lastActiveNode.get() != newNode)
	{
		if (m_lastActiveNode->hasMaterialComponent())
		{
			shared_ptr<MaterialComponent> material = m_lastActiveNode->getMaterialComponent();
			material->removeChangeListener(this);
		}
	}

	if (!m_lastActiveNode || m_lastActiveNode.get() != newNode)
	{
		material->addChangeListener(this, [this](auto newMat) {this->_refresh(newMat); });
	}
}

void MaterialWidget::_refresh(MaterialComponent *material)
{
	if (m_currentNode)
	{
		glm::vec3 color = material->getColor();
		boost::lock_guard<boost::mutex> guard(m_textLock);
		static const QRegExp trailingZeros("0+$");
		static const QRegExp trailingDot("\\.$");
		m_colorREdit->setText(QString::number(color.x, 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_colorGEdit->setText(QString::number(color.y, 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_colorBEdit->setText(QString::number(color.z, 'f', 3).remove(trailingZeros).remove(trailingDot));

		int textureId = material->getTextureImageId().get_value_or(-1);
		if (textureId >= 0)
		{
			m_textureEdit->setText(QString::fromStdString(m_resources->getPathForId(textureId).get_value_or("")));
		}
		else
		{
			m_textureEdit->setText("");
		}

		m_texScaleUEdit->setText(QString::number(material->getTexScaleU(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texScaleVEdit->setText(QString::number(material->getTexScaleV(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texOffsetUEdit->setText(QString::number(material->getTexOffsetU(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texOffsetVEdit->setText(QString::number(material->getTexOffsetV(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texRotateSlider->setValue(100.0f * material->getTexRotate() / (2.0f * PI));

		int alphaId = material->getAlphaImageId().get_value_or(-1);
		if (alphaId >= 0)
		{
			m_alphaEdit->setText(QString::fromStdString(m_resources->getPathForId(alphaId).get_value_or("")));
		}
		else
		{
			m_alphaEdit->setText("");
		}

		m_specIntensitySlider->setValue(material->getSpecIntensityPct());
		m_specShineSlider->setValue(material->getShininess());

		int specularId = material->getSpecularImageId().get_value_or(-1);
		if (specularId >= 0)
		{
			m_specMapEdit->setText(QString::fromStdString(m_resources->getPathForId(specularId).get_value_or("")));
		}
		else
		{
			m_specMapEdit->setText("");
		}
	}
}