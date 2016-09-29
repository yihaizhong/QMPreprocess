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
/**
 * @brief The PosModule class, for handling pos data
 * should set the pos file and format
 */
class PosModule
{
public:
	PosModule();
	~PosModule();
    /**
     * @brief Must call once! set the pos file
     * @param pos_file
     */
	void set_pos_file(QString &pos_file);
    /**
     * @brief Must call once! set format of pos, which decides the pos reader
     * @param format
     */
	void set_pos_format(ePosFormat &format);
    /**
     * @brief do the same thing for each pos data
     */
	void forEach(void(*fun)(PosData &pos_data));

    /**
     * @brief read pos from pos file
     */
	virtual void read();
    /**
     * @brief append content after pos.id,like .JPG
     * @param content
     */
	virtual void appendContentToId(QString &content);
    /**
     * @brief save each pos as xml file in the directory "POS_DIR/Ori-Pos"
     */
	virtual void writePosFile();
    /**
     * @brief writePairFile
     * @param pair_file
     */
	virtual void writePairFile(QString &pair_file);
private:
	QString pos_file_;
	PosReader *reader_;
	QVector<PosData> pos_data_;
};
/**
 * @brief the default pos module, use proj.4 to project if the pos format is LBH...
 */
typedef PosModule Proj4PosModule;
}
#endif // POSMODULE_H
