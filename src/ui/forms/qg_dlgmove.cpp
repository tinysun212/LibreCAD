#include "qg_dlgmove.h"

#include <qvariant.h>
#include "rs_settings.h"
#include "qg_dlgmove.ui.h"
/*
 *  Constructs a QG_DlgMove as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
QG_DlgMove::QG_DlgMove(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
    setupUi(this);

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
QG_DlgMove::~QG_DlgMove()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void QG_DlgMove::languageChange()
{
    retranslateUi(this);
}

