#ifndef POSMODULE_H
#define POSMODULE_H
#include<QVector>
#include <QString>
namespace qm{

enum ePosFormat
{
	XYZOPK,
	LBHOPK,
	XYZ,
	LBH
};
enum ePosModule
{
	PROJ4,
	GDAL
};
class PosReader;
struct PosData;
class PosModule
{
public:
	PosModule();
	~PosModule();
	void set_pos_file(QString &pos_file);
	void set_pos_format(ePosFormat &format);
	void forEach(void(*fun)(PosData &pos_data));

	virtual void readToVector();
	virtual void appendContentToId(QString &content);
	virtual void writePosFile();
	virtual void writePairFile(QString &pair_file);
private:
	QString pos_file_;
	PosReader *reader_;
	QVector<PosData> pos_data_;
};
typedef PosModule Proj4PosModule;
}
#endif // POSMODULE_H
