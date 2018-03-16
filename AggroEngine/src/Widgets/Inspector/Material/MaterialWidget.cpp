#include "MaterialWidget.hpp"
#include <QDir>
#include <QFileDialog>

MaterialWidget::MaterialWidget(QWidget *parent, shared_ptr<EngineContext> context)
	: InspectorSubWidget(parent)
	, m_colorREdit(new QLineEdit("0"))
	, m_colorGEdit(new QLineEdit("0"))
	, m_colorBEdit(new QLineEdit("0"))
	, m_textureEdit(new TextureLineEdit(context->getResources(), [this](auto material, int id) { material->setTextureImageId(id); }))
	, m_texScaleUEdit(new QLineEdit(""))
	, m_texScaleVEdit(new QLineEdit(""))
	, m_texOffsetUEdit(new QLineEdit(""))
	, m_texOffsetVEdit(new QLineEdit(""))
	, m_texRotateSlider(new QSlider(Qt::Horizontal))
	, m_normalEdit(new TextureLineEdit(context->getResources(), [this](auto material, int id) { material->setNormalImageId(id); }))
	, m_alphaEdit(new TextureLineEdit(context->getResources(), [this](auto material, int id) { material->setAlphaImageId(id); }))
	, m_specIntensitySlider(new QSlider(Qt::Horizontal))
	, m_specShineSlider(new QSlider(Qt::Horizontal))
	, m_specMapEdit(new TextureLineEdit(context->getResources(), [this](auto material, int id) { material->setSpecularImageId(id); }))
	, m_emissionREdit(new QLineEdit("0"))
	, m_emissionGEdit(new QLineEdit("0"))
	, m_emissionBEdit(new QLineEdit("0"))
	, m_emissionMapEdit(new TextureLineEdit(context->getResources(), [this](auto material, int id) {
		material->setEmissionImageId(id);
		material->setEmission(glm::vec3(1.0));
		_refresh(material);
	}))
	, m_resources(context->getResources())
{
	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *leftLayout = new QVBoxLayout;
	QVBoxLayout *rightLayout = new QVBoxLayout;
		QHBoxLayout *colorLayout = new QHBoxLayout;
		QHBoxLayout *textureLayout = new QHBoxLayout;
		QHBoxLayout *normalLayout = new QHBoxLayout;
		QHBoxLayout *alphaLayout = new QHBoxLayout;
		QHBoxLayout *specIntensityLayout = new QHBoxLayout;
		QHBoxLayout *specShineLayout = new QHBoxLayout;
		QHBoxLayout *specMapLayout = new QHBoxLayout;
		QHBoxLayout *texScaleLayout = new QHBoxLayout;
		QHBoxLayout *texOffsetLayout = new QHBoxLayout;
		QHBoxLayout *texRotateLayout = new QHBoxLayout;
		QHBoxLayout *emissionColorLayout = new QHBoxLayout;
		QHBoxLayout *emissionMapLayout = new QHBoxLayout;

	m_colorREdit->setFixedWidth(70);
	m_colorGEdit->setFixedWidth(70);
	m_colorBEdit->setFixedWidth(70);
	m_texScaleUEdit->setFixedWidth(70);
	m_texScaleVEdit->setFixedWidth(70);
	m_texOffsetUEdit->setFixedWidth(70);
	m_texOffsetVEdit->setFixedWidth(70);
	m_emissionREdit->setFixedWidth(70);
	m_emissionGEdit->setFixedWidth(70);
	m_emissionBEdit->setFixedWidth(70);

	QLabel *lbl;

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);

	leftLayout->addWidget(new QLabel("Color"));
	leftLayout->addWidget(new QLabel("Texture"));
	leftLayout->addWidget(new QLabel("Tex Scale"));
	leftLayout->addWidget(new QLabel("Tex Offset"));
	leftLayout->addWidget(new QLabel("Tex Rotate"));
	leftLayout->addWidget(new QLabel("Normal"));
	leftLayout->addWidget(new QLabel("Alpha"));
	leftLayout->addWidget(new QLabel("Spec Intensity"));
	leftLayout->addWidget(new QLabel("Spec Shininess"));
	leftLayout->addWidget(new QLabel("Spec Map"));
	leftLayout->addWidget(new QLabel("Emission Color"));
	leftLayout->addWidget(new QLabel("Emission Map"));

	rightLayout->addLayout(colorLayout);
	rightLayout->addLayout(textureLayout);
	rightLayout->addLayout(texScaleLayout);
	rightLayout->addLayout(texOffsetLayout);
	rightLayout->addLayout(texRotateLayout);
	rightLayout->addLayout(normalLayout);
	rightLayout->addLayout(alphaLayout);
	rightLayout->addLayout(specIntensityLayout);
	rightLayout->addLayout(specShineLayout);
	rightLayout->addLayout(specMapLayout);
	rightLayout->addLayout(emissionColorLayout);
	rightLayout->addLayout(emissionMapLayout);

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

	QPushButton *selectTexButton = _addTexSelect(textureLayout, m_textureEdit);

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

	QPushButton *selectNormalButton = _addTexSelect(normalLayout, m_normalEdit);
	QPushButton *selectAlphaButton = _addTexSelect(alphaLayout, m_alphaEdit);

	m_specIntensitySlider->setStyleSheet(QString("border-bottom-style: none;"));
	specIntensityLayout->addWidget(m_specIntensitySlider.get());
	m_specShineSlider->setMinimum(5);
	m_specShineSlider->setMaximum(100);
	m_specShineSlider->setStyleSheet(QString("border-bottom-style: none;"));
	specShineLayout->addWidget(m_specShineSlider.get());
	QPushButton *selectSpecButton = _addTexSelect(specMapLayout, m_specMapEdit);

	// Emission
	lbl = new QLabel("R");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	emissionColorLayout->addWidget(lbl);
	emissionColorLayout->addWidget(m_emissionREdit.get());
	lbl = new QLabel("G");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	emissionColorLayout->addWidget(lbl);
	emissionColorLayout->addWidget(m_emissionGEdit.get());
	lbl = new QLabel("B");
	lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	emissionColorLayout->addWidget(lbl);
	emissionColorLayout->addWidget(m_emissionBEdit.get());

	QPushButton *selectEmissionButton = _addTexSelect(emissionMapLayout, m_emissionMapEdit);

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
	connect(selectNormalButton, &QPushButton::pressed, this, [this]() {
		this->_onTexSelect(m_normalEdit,
			[this](auto material, int id) { material->setNormalImageId(id); },
			[this](auto material) { material->removeNormalMap(); }
		);
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
	connect(m_emissionREdit.get(), &QLineEdit::textEdited, this, &MaterialWidget::_onEmissionChange);
	connect(m_emissionGEdit.get(), &QLineEdit::textEdited, this, &MaterialWidget::_onEmissionChange);
	connect(m_emissionBEdit.get(), &QLineEdit::textEdited, this, &MaterialWidget::_onEmissionChange);
	connect(selectEmissionButton, &QPushButton::pressed, this, [this]() {
		this->_onTexSelect(m_emissionMapEdit,
			[this](auto material, int id) { 
				material->setEmissionImageId(id); 
				material->setEmission(glm::vec3(1.0));
				this->_refresh(material.get());
			},
			[this](auto material) { 
				material->removeEmissionMap();
				material->setEmission(glm::vec3(0));
				this->_refresh(material.get());
			}
		);
	});
}

QPushButton *MaterialWidget::_addTexSelect(QHBoxLayout *layout, shared_ptr<QLineEdit> lineEdit)
{
	QPushButton *button = new QPushButton("...");
	button->setFixedWidth(50);
	button->setFixedHeight(25);
	layout->addWidget(lineEdit.get());
	layout->addWidget(button);
	return button;
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

void MaterialWidget::_onEmissionChange(QString newValue)
{
	boost::lock_guard<MaterialWidget> guard(*this);
	if (m_currentNode && m_currentNode->hasMaterialComponent())
	{
		shared_ptr<MaterialComponent> material = m_currentNode->getMaterialComponent();
		material->removeChangeListener(this);
		material->setEmission(glm::vec3(
			m_emissionREdit->text().toFloat(),
			m_emissionGEdit->text().toFloat(),
			m_emissionBEdit->text().toFloat()
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
		newNode->getMaterialComponent()->getOwner() != newNode ||
		newNode->hasCameraComponent())
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
		m_textureEdit->setMaterial(material);
		m_normalEdit->setMaterial(material);
		m_alphaEdit->setMaterial(material);
		m_specMapEdit->setMaterial(material);
		m_emissionMapEdit->setMaterial(material);

		glm::vec3 color = material->getColor();
		boost::lock_guard<boost::mutex> guard(m_textLock);
		static const QRegExp trailingZeros("0+$");
		static const QRegExp trailingDot("\\.$");
		m_colorREdit->setText(QString::number(color.x, 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_colorGEdit->setText(QString::number(color.y, 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_colorBEdit->setText(QString::number(color.z, 'f', 3).remove(trailingZeros).remove(trailingDot));

		_setTexEdit(m_textureEdit, material->getTextureImageId());

		m_texScaleUEdit->setText(QString::number(material->getTexScaleU(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texScaleVEdit->setText(QString::number(material->getTexScaleV(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texOffsetUEdit->setText(QString::number(material->getTexOffsetU(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texOffsetVEdit->setText(QString::number(material->getTexOffsetV(), 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_texRotateSlider->setValue(100.0f * material->getTexRotate() / (2.0f * PI));

		_setTexEdit(m_normalEdit, material->getNormalImageId());
		_setTexEdit(m_alphaEdit, material->getAlphaImageId());

		m_specIntensitySlider->setValue(material->getSpecIntensityPct());
		m_specShineSlider->setValue(material->getShininess());
		_setTexEdit(m_specMapEdit, material->getSpecularImageId());

		glm::vec3 emission = material->getEmission();
		m_emissionREdit->setText(QString::number(emission.x, 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_emissionGEdit->setText(QString::number(emission.y, 'f', 3).remove(trailingZeros).remove(trailingDot));
		m_emissionBEdit->setText(QString::number(emission.z, 'f', 3).remove(trailingZeros).remove(trailingDot));

		_setTexEdit(m_emissionMapEdit, material->getEmissionImageId());
	}
}

void MaterialWidget::_setTexEdit(shared_ptr<QLineEdit> texEdit, boost::optional<int> imageId)
{
	if (imageId)
	{
		texEdit->setText(QString::fromStdString(m_resources->getPathForId(*imageId).get_value_or("")));
	}
	else
	{
		texEdit->setText("");
	}
}