#ifndef IMAGEMODULE_H
#define IMAGEMODULE_H
#include <QString>
namespace qm{


class SensorInfo;
class ImageModule
{
public:
	ImageModule();
	~ImageModule();
	ImageModule(const ImageModule&);
	ImageModule &operator=(const ImageModule&);
	void set_image_file(QString &image_file);
	SensorInfo* get_sensor_info()const;
	void set_sensor_info(SensorInfo *sensor_info);

    /**
     * @brief setHorizontal
     */
	virtual void setHorizontal();
    /**
     * @brief checkSensorInfo, check if it can get sensor information from image
     * @return
     */
	virtual bool checkSensorInfo();
    /**
     * @brief write image information in IMAGE_DIR/Tmp-MM-Dir/IMAGE_NAME-MTD-4227.xml
     */
	virtual void writeInfoFile();
    /**
     * @brief enhance the image
     */
	virtual void enhance();

private:
	QString image_file_;
	SensorInfo *sensor_info_;
	
};
/**
 * @brief Exiv2ImageModule, use exiv2 library to read image information
 */
typedef ImageModule Exiv2ImageModule;
}
#endif // IMAGEMODULE_H
