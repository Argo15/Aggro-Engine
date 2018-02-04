#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include "InspectorSubWidget.hpp"
#include "Locks.hpp"

class MaterialWidget : public InspectorSubWidget,
	public boost::basic_lockable_adapter<recursive_mutex>
{
private:
	shared_ptr<QLineEdit> m_colorREdit;
	shared_ptr<QLineEdit> m_colorGEdit;
	shared_ptr<QLineEdit> m_colorBEdit;
	shared_ptr<QLineEdit> m_textureEdit;
	shared_ptr<QLineEdit> m_texScaleUEdit;
	shared_ptr<QLineEdit> m_texScaleVEdit;
	shared_ptr<QLineEdit> m_texOffsetUEdit;
	shared_ptr<QLineEdit> m_texOffsetVEdit;
	shared_ptr<QSlider> m_texRotateSlider;
	shared_ptr<QLineEdit> m_normalEdit;
	shared_ptr<QLineEdit> m_alphaEdit;
	shared_ptr<QSlider> m_specIntensitySlider;
	shared_ptr<QSlider> m_specShineSlider;
	shared_ptr<QLineEdit> m_specMapEdit;
	shared_ptr<QLineEdit> m_emissionREdit;
	shared_ptr<QLineEdit> m_emissionGEdit;
	shared_ptr<QLineEdit> m_emissionBEdit;
	shared_ptr<QLineEdit> m_emissionMapEdit;
	shared_ptr<Resources> m_resources;
	boost::mutex m_textLock;

protected:
	virtual void _refresh(SceneNode *newNode);
	void _refresh(MaterialComponent *material);
	void _onColorChange(QString newValue);
	void _onValueSelect(QString newValue, function<void(shared_ptr<MaterialComponent>, float)> acceptFunc);
	void _onTexSelect(shared_ptr<QLineEdit> textureEdit, 
					  function<void(shared_ptr<MaterialComponent>, int)> setImageFunc,
					  function<void(shared_ptr<MaterialComponent>)> removeFunc);
	void _onSliderChange(int value, function<void(shared_ptr<MaterialComponent>, int)> acceptFunc);
	void _onSpecularChange(int value);
	void _onShininessChange(int value);
	void _onEmissionChange(QString newValue);

	void _setTexEdit(shared_ptr<QLineEdit> texEdit, boost::optional<int> imageId);
	QPushButton *_addTexSelect(QHBoxLayout *layout, shared_ptr<QLineEdit> lineEdit);

public:
	MaterialWidget(QWidget *parent, shared_ptr<Resources> resources);
};