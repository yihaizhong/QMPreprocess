#ifndef SENSORINFO_H
#define SENSORINFO_H
#include "qmpreprocess_global.h"
#include <QString>
namespace qm{


class DLL_EXPORT SensorInfo
{
public:
	SensorInfo();
	SensorInfo(const SensorInfo& sensor_info);
	SensorInfo& operator=(const SensorInfo& sensor_info);
	~SensorInfo();
	QString get_camera_type()const;
	void set_camera_type(QString &camera_type);
	QString get_bay_pat()const;
	QString get_orientation()const;
	double get_focal_length()const;
	void set_focal_length(double focal_length);
	double get_focal_length35()const;
	void set_focal_length35(double focal_length35);
	int get_width()const;
	int get_height()const;

	double get_width_mm()const;
	void set_width_mm(double width_mm);
	double get_height_mm()const;
	void set_height_mm(double height_mm);
	/**
	* @brief readExifInfo,use exiv2 to read
	* @param image_file
	*/
	virtual void readExifInfo(QString &image_file);
	/**
	* @brief writeToFile, the default is xml
	* @param info_file
	*/
	virtual void writeToFile(QString &info_file);
protected:
	QString camera_type_;
	QString bay_pat_;
	QString orientation_;
	double focal_length_;
	double focal_length35_;
	int width_;
	int height_;
	double width_mm_;
	double height_mm_;
};
}
#endif // SENSORINFO_H
