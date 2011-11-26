/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
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
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/


#include "rs_circle.h"

//#include <values.h>

#include "rs_constructionline.h"
#include "rs_information.h"
#include "rs_graphicview.h"
#include "rs_painter.h"
#include "rs_linetypepattern.h"



/**
 * Default constructor.
 */
RS_Circle::RS_Circle(RS_EntityContainer* parent,
                     const RS_CircleData& d)
    :RS_AtomicEntity(parent), data(d) {
    calculateBorders();
}



void RS_Circle::calculateBorders() {
    RS_Vector r(data.radius,data.radius);
    minV = data.center - r;
    maxV = data.center + r;
}



/**
 * @return Angle length in rad.
 */
double RS_Circle::getAngleLength() const {
    return 2*M_PI;
}



/**
 * @return Length of the circle which is the circumference.
 */
double RS_Circle::getLength() const {
    return 2*M_PI*data.radius;
}


/**
 * Creates this circle from a center point and a radius.
 *
 * @param c Center.
 * @param r Radius
 */
bool RS_Circle::createFromCR(const RS_Vector& c, double r) {
    if (fabs(r)>RS_TOLERANCE && c.valid ) {
        data.radius = fabs(r);
        data.center = c;
        return true;
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFromCR(): "
                        "Cannot create a circle with radius 0.0.");
        return false;
    }
}



/**
 * Creates this circle from two opposite points.
 *
 * @param p1 1st point.
 * @param p2 2nd point.
 */
bool RS_Circle::createFrom2P(const RS_Vector& p1, const RS_Vector& p2) {
        double r=0.5*p1.distanceTo(p2);
    if (r>RS_TOLERANCE) {
        data.radius = r;
        data.center = (p1+p2)*0.5;
        return true;
    } else {
//        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom2P(): "
//                        "Cannot create a circle with radius 0.0.");
        return false;
    }
}



/**
 * Creates this circle from 3 given points which define the circle line.
 *
 * @param p1 1st point.
 * @param p2 2nd point.
 * @param p3 3rd point.
 */
bool RS_Circle::createFrom3P(const RS_Vector& p1, const RS_Vector& p2,
                             const RS_Vector& p3) {
        RS_Vector vra=p2 - p1;
        RS_Vector vrb=p3 - p1;
        double ra2=vra.squared()*0.5;
        double rb2=vrb.squared()*0.5;
        double crossp=vra.x * vrb.y - vra.y * vrb.x;
        if (fabs(crossp)< RS_TOLERANCE*RS_TOLERANCE) {
                RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom3P(): "
                        "Cannot create a circle with radius 0.0.");
                return false;
        }
        crossp=1./crossp;
        data.center.set((ra2*vrb.y - rb2*vra.y)*crossp,(rb2*vra.x - ra2*vrb.x)*crossp);
        data.radius=data.center.magnitude();
        data.center += p1;
        return true;
}
//*create Circle from 3 points
//Author: Dongxu Li
bool RS_Circle::createFrom3P(const RS_VectorSolutions& sol) {
    if(sol.getNumber() < 2) return false;
    if(sol.getNumber() == 2) return createFrom2P(sol.get(0),sol.get(1));
    if((sol.get(1)-sol.get(2)).squared() < RS_TOLERANCE*RS_TOLERANCE)
        return createFrom2P(sol.get(0),sol.get(1));
    RS_Vector vra(sol.get(1) - sol.get(0));
    RS_Vector vrb(sol.get(2) - sol.get(0));
    double ra2=vra.squared()*0.5;
    double rb2=vrb.squared()*0.5;
    double crossp=vra.x * vrb.y - vra.y * vrb.x;
    if (fabs(crossp)< RS_TOLERANCE*RS_TOLERANCE) {
        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom3P(): "
                        "Cannot create a circle with radius 0.0.");
        return false;
    }
    crossp=1./crossp;
    data.center.set((ra2*vrb.y - rb2*vra.y)*crossp,(rb2*vra.x - ra2*vrb.x)*crossp);
    data.radius=data.center.magnitude();
    data.center += sol.get(0);
    return true;
}
//
//    if (p1.distanceTo(p2)>RS_TOLERANCE &&
//            p2.distanceTo(p3)>RS_TOLERANCE &&
//            p3.distanceTo(p1)>RS_TOLERANCE) {
//
//        // middle points between 3 points:
//        RS_Vector mp1, mp2;
//        RS_Vector dir1, dir2;
//        double a1, a2;
//
//        // intersection of two middle lines
//        mp1 = (p1 + p2)/2.0;
//        a1 = p1.angleTo(p2) + M_PI/2.0;
//        dir1.setPolar(100.0, a1);
//        mp2 = (p2 + p3)/2.0;
//        a2 = p2.angleTo(p3) + M_PI/2.0;
//        dir2.setPolar(100.0, a2);
//
//        RS_ConstructionLineData d1(mp1, mp1 + dir1);
//        RS_ConstructionLineData d2(mp2, mp2 + dir2);
//        RS_ConstructionLine midLine1(NULL, d1);
//        RS_ConstructionLine midLine2(NULL, d2);
//
//        RS_VectorSolutions sol =
//            RS_Information::getIntersection(&midLine1, &midLine2);
//
//        data.center = sol.get(0);
//        data.radius = data.center.distanceTo(p3);
//
//        if (sol.get(0).valid && data.radius<1.0e14 && data.radius>RS_TOLERANCE) {
//            return true;
//        } else {
//            RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom3P(): "
//                            "Cannot create a circle with inf radius.");
//            return false;
//        }
//    } else {
//        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_Circle::createFrom3P(): "
//                        "Cannot create a circle with radius 0.0.");
//        return false;
//    }
//}
/**
  *create circle inscribled in a triangle
  *
  *Author: Dongxu Li
  */
bool RS_Circle::createInscribe(const RS_Vector& coord, const QVector<RS_Line*>& lines){
    if(lines.size()<3) return false;
    QVector<RS_Line*> tri(lines);
    RS_VectorSolutions sol=RS_Information::getIntersectionLineLine(tri[0],tri[1]);
    if(sol.getNumber() == 0 ) {//move parallel to opposite
        std::swap(tri[1],tri[2]);
        sol=RS_Information::getIntersectionLineLine(tri[0],tri[1]);
    }
    if(sol.getNumber() == 0 ) return false;
    RS_Vector vp0(sol.get(0));
    sol=RS_Information::getIntersectionLineLine(tri[2],tri[1]);
    if(sol.getNumber() == 0 ) return false;
    RS_Vector vp1(sol.get(0));
    RS_Vector dvp(vp1-vp0);
    double a(dvp.squared());
    if( a< RS_TOLERANCE*RS_TOLERANCE) return false; //three lines share a common intersecting point
    RS_Vector vp(coord - vp0);
    vp -= dvp*(RS_Vector::dotP(dvp,vp)/a); //normal component
    RS_Vector vl0(tri[0]->getEndpoint() - tri[0]->getStartpoint());
    a=dvp.angle();
    double angle0(0.5*(vl0.angle() + a));
    if( RS_Vector::dotP(vp,vl0) <0.) {
        angle0 += 0.5*M_PI;
    }

    RS_Line line0(vp0, vp0+RS_Vector(angle0));//first bisecting line
    vl0=(tri[2]->getEndpoint() - tri[2]->getStartpoint());
    angle0=0.5*(vl0.angle() + a+M_PI);
    if( RS_Vector::dotP(vp,vl0) <0.) {
        angle0 += 0.5*M_PI;
    }
    RS_Line line1(vp1, vp1+RS_Vector(angle0));//second bisection line
    sol=RS_Information::getIntersectionLineLine(&line0,&line1);
    if(sol.getNumber() == 0 ) return false;
    return createFromCR(sol.get(0),tri[1]->getDistanceToPoint(sol.get(0)));
}


RS_VectorSolutions RS_Circle::getRefPoints() {
    RS_Vector v1(data.radius, 0.0);
    RS_Vector v2(0.0, data.radius);

    RS_VectorSolutions ret(data.center,
                           data.center+v1, data.center+v2,
                           data.center-v1, data.center-v2);
    return ret;
}


/**
 * @return Always an invalid vector.
 */
RS_Vector RS_Circle::getNearestEndpoint(const RS_Vector& /*coord*/, double* dist)const {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



RS_Vector RS_Circle::getNearestPointOnEntity(const RS_Vector& coord,
        bool /*onEntity*/, double* dist, RS_Entity** entity)const {

    if (entity!=NULL) {
        *entity = const_cast<RS_Circle*>(this);
    }
    RS_Vector vp(coord - data.center);
    double d(vp.magnitude());
    if( d < RS_TOLERANCE ) return RS_Vector(false);
    vp =data.center+vp*(data.radius/d);
    if(dist!=NULL){
        *dist=coord.distanceTo(vp);
    }
    return vp;
}


/**
  *find the tangential points from a given point, i.e., the tangent lines should pass
  * the given point and tangential points
  *
  *Author: Dongxu Li
  */
RS_VectorSolutions RS_Circle::getTangentPoint(const RS_Vector& point) const {
    RS_VectorSolutions ret;
    double r2(getRadius()*getRadius());
    if(r2<RS_TOLERANCE*RS_TOLERANCE) return ret; //circle too small
    RS_Vector vp(point-getCenter());
    double c2(vp.squared());
    if(c2<r2-getRadius()*2.*RS_TOLERANCE) {
        //inside point, no tangential point
        return ret;
    }
    if(c2>r2+getRadius()*2.*RS_TOLERANCE) {
        //external point
        RS_Vector vp1(-vp.y,vp.x);
        vp1*=getRadius()*sqrt(c2-r2)/c2;
        vp *= r2/c2;
        vp += getCenter();
        if(vp1.squared()>RS_TOLERANCE*RS_TOLERANCE) {
            ret.push_back(vp+vp1);
            ret.push_back(vp-vp1);
            return ret;
        }
    }
    ret.push_back(point);
    return ret;
}
RS_Vector RS_Circle::getTangentDirection(const RS_Vector& point) const {
    RS_Vector vp(point-getCenter());
//    double c2(vp.squared());
//    if(c2<r2-getRadius()*2.*RS_TOLERANCE) {
//        //inside point, no tangential point
//        return RS_Vector(false);
//    }
    return RS_Vector(-vp.y,vp.x);

}

RS_Vector RS_Circle::getNearestCenter(const RS_Vector& coord,
                                      double* dist) {
    if (dist!=NULL) {
        *dist = coord.distanceTo(data.center);
    }
    return data.center;
}



RS_Vector RS_Circle::getMiddlePoint(void)const
{
    return RS_Vector(false);
}

RS_Vector RS_Circle::getNearestMiddle(const RS_Vector& /*coord*/,
                                      double* dist,
                                      const int /*middlePoints*/
                                      )const {
    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}



RS_Vector RS_Circle::getNearestDist(double /*distance*/,
                                    const RS_Vector& /*coord*/,
                                    double* dist) {

    if (dist!=NULL) {
        *dist = RS_MAXDOUBLE;
    }
    return RS_Vector(false);
}

RS_Vector RS_Circle::getNearestDist(double /*distance*/,
                                    bool /*startp*/) {

    return RS_Vector(false);
}


RS_Vector RS_Circle::getNearestOrthTan(const RS_Vector& coord,
                    const RS_Line& normal,
                    bool /*onEntity = false*/)
{
        if ( !coord.valid) {
                return RS_Vector(false);
        }
        RS_Vector vp0(coord-getCenter());
        RS_Vector vp1(normal.getAngle1());
        double d=RS_Vector::dotP(vp0,vp1);
        if(d >= 0. ) {
                return getCenter() + vp1*getRadius();
        }else{
                return getCenter() - vp1*getRadius();
        }
}

double RS_Circle::getDistanceToPoint(const RS_Vector& coord,
                                     RS_Entity** entity,
                                     RS2::ResolveLevel, double) const {
    if (entity!=NULL) {
        *entity = const_cast<RS_Circle*>(this);
    }

    // RVT Jan 6 2010, allow selections to mid point of circle
    double dToCenter=data.center.distanceTo(coord);
    double dToEdge=fabs(dToCenter - data.radius);

    if (dToEdge<dToCenter) {
        return dToEdge;
    } else {
        return dToCenter;
    }
}



void RS_Circle::move(const RS_Vector& offset) {
    data.center.move(offset);
    moveBorders(offset);
//    calculateBorders();
}

/**
  * this function creates offset
  *@coord, position indicates the direction of offset
  *@distance, distance of offset
  * return true, if success, otherwise, false
  *
  *Author: Dongxu Li
  */
bool RS_Circle::offset(const RS_Vector& coord, const double& distance) {
    double r0(coord.distanceTo(getCenter()));
    if(r0 > getRadius()){
        //external
        r0 = getRadius()+ fabs(distance);
    }else{
        r0 = getRadius()- fabs(distance);
        if(r0<RS_TOLERANCE) {
            return false;
        }
    }
    setRadius(r0);
    calculateBorders();
    return true;
}

void RS_Circle::rotate(const RS_Vector& center, const double& angle) {
    data.center.rotate(center, angle);
    calculateBorders();
}

void RS_Circle::rotate(const RS_Vector& center, const RS_Vector& angleVector) {
    data.center.rotate(center, angleVector);
    calculateBorders();
}

void RS_Circle::scale(const RS_Vector& center, const RS_Vector& factor) {
    data.center.scale(center, factor);
    data.radius *= factor.x;
    scaleBorders(center,factor);
//    calculateBorders();
}



void RS_Circle::mirror(const RS_Vector& axisPoint1, const RS_Vector& axisPoint2) {
    data.center.mirror(axisPoint1, axisPoint2);
    calculateBorders();
}


void RS_Circle::draw(RS_Painter* painter, RS_GraphicView* view, double& /*patternOffset*/) {

    if (painter==NULL || view==NULL) {
        return;
    }
    RS_Vector cp(view->toGui(getCenter()));
    double ra(fabs(getRadius()*view->getFactor().x));
    //double styleFactor = getStyleFactor();

    // simple style-less lines
    if ( !isSelected() && (
             getPen().getLineType()==RS2::SolidLine ||
             view->getDrawingMode()==RS2::ModePreview)) {

        painter->drawArc(cp,
                         ra,
                         0.0, 2*M_PI,
                         false);
        return;
    }
//    double styleFactor = getStyleFactor(view);
    //        if (styleFactor<0.0) {
    //            painter->drawArc(cp,
    //                             ra,
    //                             0.0, 2*M_PI,
    //                             false);
    //            return;
    //        }

    // Pattern:
    RS_LineTypePattern* pat;
    if (isSelected()) {
        pat = &patternSelected;
    } else {
        pat = view->getPattern(getPen().getLineType());
    }

    if (pat==NULL) {
        return;
    }

    if (ra<1.){
        painter->drawArc(cp,
                         ra,
                         0.0, 2*M_PI,
                         false);
        return;
    }

    // Pen to draw pattern is always solid:
    RS_Pen pen = painter->getPen();
    pen.setLineType(RS2::SolidLine);
    painter->setPen(pen);

    // create pattern:
    double* da=new double[pat->num>0?pat->num:0];
    int i(0),j(0);          // index counter
    if(pat->num>0){
        while(i<pat->num){
            //        da[j] = pat->pattern[i++] * styleFactor;
            //fixme, styleFactor needed
            da[i] = pat->pattern[i]/ra ;
            i++;
        }
        j=i;
    }else {
        //invalid pattern
        delete[] da;
        painter->drawArc(cp,
                         ra,
                         0.,2.*M_PI,
                         false);
        return;
    }

    double curA ( 0.0);
    double a2;
    bool notDone=true;

    for(i=0;notDone;i=(i+1)%j) {
        a2= curA+fabs(da[i]);
        if(a2>2.*M_PI) {
            a2=2.*M_PI;
            notDone=false;
        }
        if (da[i]>0.){
            painter->drawArc(cp, ra,
                             curA,
                             a2,
                             false);
        }
        curA=a2;
    }

    delete[] da;
}



void RS_Circle::moveRef(const RS_Vector& ref, const RS_Vector& offset) {
    RS_Vector v1(data.radius, 0.0);
    RS_Vector v2(0.0, data.radius);

    if (ref.distanceTo(data.center + v1)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center + v1 + offset);
    } else if (ref.distanceTo(data.center + v2)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center + v2 + offset);
    } else if (ref.distanceTo(data.center - v1)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center - v1 + offset);
    } else if (ref.distanceTo(data.center - v2)<1.0e-4) {
        data.radius = data.center.distanceTo(data.center - v2 + offset);
    }
}


/**
 * Dumps the circle's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_Circle& a) {
    os << " Circle: " << a.data << "\n";
    return os;
}

