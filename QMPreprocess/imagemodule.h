#ifndef IMAGEMODULE_H
#define IMAGEMODULE_H
#include <QString>
namespace qm{

enum eImageModule
{
	EXIV2,
	EXIFTOOL
};
class SensorInfo;
class ImageModule
{
public:
	ImageModule();
	~ImageModule();
	void set_image_file(QString &image_file);
	SensorInfo* get_sensor_info()const;
	void set_sensor_info(SensorInfo *sensor_info);

	virtual void setHorizontal();
	virtual bool checkSensorInfo();
	virtual void writeInfoFile();
	virtual void enhance();

private:
	QString image_file_;
	SensorInfo *sensor_info_;
	
};
typedef ImageModule Exiv2ImageModule;
}
#endif // IMAGEMODULE_H
