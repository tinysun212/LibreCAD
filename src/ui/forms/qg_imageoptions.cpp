#include "qg_imageoptions.h"

#include <qvariant.h>
#include "qg_imageoptions.ui.h"
/*
 *  Constructs a QG_ImageOptions as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
QG_ImageOptions::QG_ImageOptions(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : QWidget(parent, name, fl)
{
    setupUi(this);

}

/*
 *  Destroys the object and frees any allocated resources
 */
QG_ImageOptions::~QG_ImageOptions()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_ImageOptions::languageChange()
{
    retranslateUi(this);
}

