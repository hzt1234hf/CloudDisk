#include "transferpanel.h"

TransferPanel::TransferPanel(QWidget* parent)
{
    setTabBar(new TransferPanelTabBar);
    setTabPosition(QTabWidget::West);
}
