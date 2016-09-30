#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "qmpreprocess_global.h"
class QString;
template<class T>
class QVector;
namespace qm{
class SensorInfo;
class ImageModule;
class PosModule;

class DLL_EXPORT Preprocess
{
public:
	enum eImageModule
	{
		EXIV2,
		EXIFTOOL
	};

	enum ePosModule
	{
		PROJ4,
		GDAL
	};
	Preprocess(QVector<QString>& image_files);
	~Preprocess();
	void setModule(eImageModule image_module,ePosModule pos_module);

	void setImageHorizontal();
	bool checkSensorInfoInImage(QString &image_file);
	void setSensor(SensorInfo *sensor);
	void makeSensorFile(QString &sensor_file);

	void makeImageInfoFile();

	void readPos(QString &pos_file,ePosFormat pos_foramt);
	void makeImagePosFile();
	void makeImagePairFile(QString &pair_file);

	void enhanceImage();

private:
	QVector<QString> &image_files_;
	ImageModule *image_module_;
	PosModule *pos_module_;
};
}
#endif // PREPROCESS_H
