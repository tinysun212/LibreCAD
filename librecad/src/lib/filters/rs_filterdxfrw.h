/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
**  Copyright (C) 2011 Rallaz, rallazz@gmail.com
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License as published by the Free Software
** Foundation either version 2 of the License, or (at your option)
**  any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**********************************************************************/


#ifndef RS_FILTERDXFRW_H
#define RS_FILTERDXFRW_H

#include "rs_filterinterface.h"

#include "rs_block.h"
#include "rs_color.h"
#include "rs_dimension.h"
#include "rs_dimaligned.h"
#include "rs_insert.h"
#include "rs_layer.h"
#include "rs_leader.h"
#include "rs_polyline.h"
#include "rs_solid.h"
#include "rs_text.h"
#include "rs_image.h"

#include "drw_interface.h"
#include "libdxfrw.h"

class RS_Spline;
class RS_Hatch;
class DL_WriterA;

/**
 * This format filter class can import and export DXF files.
 * It depends on the dxflib library.
 *
 * @author Rallaz
 */
class RS_FilterDXFRW : public RS_FilterInterface, DRW_Interface {
public:
    RS_FilterDXFRW();
    ~RS_FilterDXFRW();
	
	virtual bool canImport(const QString &fileName, RS2::FormatType t) const {
        return (t==RS2::FormatDXFRW);
	}
	
	virtual bool canExport(const QString &fileName, RS2::FormatType t) const {
        return (t==RS2::FormatDXFRW || t==RS2::FormatDXF12);
    }

    // Import:
    virtual bool fileImport(RS_Graphic& g, const QString& file, RS2::FormatType /*type*/);

    // Methods from DRW_CreationInterface:
    virtual void addHeader(const DRW_Header* data);
    virtual void addLType(const DRW_LType& /*data*/){}
    virtual void addLayer(const DRW_Layer& data);
    virtual void addBlock(const DRW_Block& data);
    virtual void endBlock();
    virtual void addPoint(const DRW_Point& data);
    virtual void addLine(const DRW_Line& data);
    virtual void addCircle(const DRW_Circle& data);
    virtual void addArc(const DRW_Arc& data);
    virtual void addEllipse(const DRW_Ellipse& data);
    virtual void addLWPolyline(const DRW_LWPolyline& data);
    virtual void addText(const DRW_Text& data);
    virtual void addPolyline(const DRW_Polyline& data);
    virtual void addSpline(const DRW_Spline* data);
    virtual void addKnot(const DRW_Entity&) {}
    virtual void addInsert(const DRW_Insert& data);
    virtual void addTrace(const DRW_Trace& data);
    virtual void addSolid(const DRW_Solid& data);
    virtual void addMText(const DRW_MText& data);
    RS_DimensionData convDimensionData(const DRW_Dimension* data);
    virtual void addDimAlign(const DRW_DimAligned *data);
    virtual void addDimLinear(const DRW_DimLinear *data);
    virtual void addDimRadial(const DRW_DimRadial *data);
    virtual void addDimDiametric(const DRW_DimDiametric *data);
    virtual void addDimAngular(const DRW_DimAngular *data);
    virtual void addDimAngular3P(const DRW_DimAngular3p *data);
    virtual void addDimOrdinate(const DRW_DimOrdinate *data);
    virtual void addLeader(const DRW_Leader *data);
    virtual void addHatch(const DRW_Hatch* data);
    virtual void addImage(const DRW_Image* data);
    virtual void linkImage(const DRW_ImageDef* data);

    virtual void add3dFace(const DRW_3Dface& data);
    virtual void addComment(const char*);

    // Export:
    virtual bool fileExport(RS_Graphic& g, const QString& file, RS2::FormatType type);

    virtual void writeHeader(DRW_Header& data);
    virtual void writeEntities();
    virtual void writeLTypes();
    virtual void writeLayers();
    virtual void writeBlockRecords();
    virtual void writeBlocks();

//    void writeVariables(DL_WriterA& dw);
//    void writeLayer(DL_WriterA& dw, RS_Layer* l);
//    void writeAppid(DL_WriterA& dw, const char* appid);
//    void writeBlock(DL_WriterA& dw, RS_Block* blk);

    void writePoint(RS_Point* p);
    void writeLine(RS_Line* l);
    void writeCircle(RS_Circle* c);
    void writeArc(RS_Arc* a);
    void writeEllipse(RS_Ellipse* s);
    void writeSolid(RS_Solid* s);
    void writeLWPolyline(RS_Polyline* l);
    void writeSpline(RS_Spline* s);
    void writeInsert(RS_Insert* i);
    void writeText(RS_Text* t);
    void writeHatch(RS_Hatch* h);
    void writeImage(RS_Image* i);
    void writeDimAligned(RS_DimAligned* d);

    void writePolyline(DL_WriterA& dw,
                RS_Polyline* l, const DRW_Entity& attrib);
	void writeDimension(DL_WriterA& dw, RS_Dimension* d, 
                const DRW_Entity& attrib);
        void writeLeader(DL_WriterA& dw, RS_Leader* l, const DRW_Entity& attrib);
	void writeEntityContainer(DL_WriterA& dw, RS_EntityContainer* con, 
                const DRW_Entity& attrib);
	void writeAtomicEntities(DL_WriterA& dw, RS_EntityContainer* c, 
                const DRW_Entity& attrib, RS2::ResolveLevel level);
	

    void setEntityAttributes(RS_Entity* entity, const DRW_Entity* attrib);
    void getEntityAttributes(DRW_Entity* ent, const RS_Entity* entity);

    static QString toDxfString(const QString& str);
    static QString toNativeString(const char* data, const QString& encoding);
    QString getDXFEncoding();

public:
    RS_Pen attributesToPen(const DRW_Layer* att) const;

    static RS_Color numberToColor(int num, bool comp=false);
    static int colorToNumber(const RS_Color& col);

    static RS2::LineType nameToLineType(const QString& name);
    static QString lineTypeToName(RS2::LineType lineType);
    //static QString lineTypeToDescription(RS2::LineType lineType);

    static RS2::LineWidth numberToWidth(int num);
    static int widthToNumber(RS2::LineWidth width);

	static RS2::AngleFormat numberToAngleFormat(int num);
	static int angleFormatToNumber(RS2::AngleFormat af);

	static RS2::Unit numberToUnit(int num);
	static int unitToNumber(RS2::Unit unit);
	
        static bool isVariableTwoDimensional(const QString& var);

    static RS_FilterInterface* createFilter(){return new RS_FilterDXFRW();}

private:
    void writeEntity(RS_Entity* e);

private:
    /** Pointer to the graphic we currently operate on. */
    RS_Graphic* graphic;
    /** File name. Used to find out the full path of images. */
    QString file;
    /** Pointer to current entity container (either block or graphic) */
    RS_EntityContainer* currentContainer;
    /** File codePage. Used to find the text coder. */
    QString codePage;
    /** File version. */
    QString versionStr;
    int version;
    /** dimension style. */
    QString dimStyle;
    /** text style. */
    QString textStyle;

    dxfRW *dxf;
};

#endif
