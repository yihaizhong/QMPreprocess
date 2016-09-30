#ifndef SENSORINFODB_H
#define SENSORINFODB_H
#include <QMap>

namespace qm{


class SensorInfo;
class SensorInfoDB
{
public:
	~SensorInfoDB();
	static SensorInfoDB* instance();
	bool isInDB(SensorInfo *sensor_info);
	void addSensorInfo(SensorInfo *sensor_info);
	void set_db_file(QString &db_file);
private:
	void readFromDBFile();
	void writeToDBFile();
	SensorInfoDB();
	SensorInfoDB(const SensorInfoDB&);
	SensorInfoDB &operator=(const SensorInfoDB&);
	static SensorInfoDB* instance_;
	static QString db_file_;
	class GC
	{
	public:
		GC();
		~GC();
	};
	static GC gc;
	QMap<QString,SensorInfo*> sensor_db_;
};
}
#endif // SENSORINFODB_H
