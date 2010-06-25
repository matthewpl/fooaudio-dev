#include "foosettingsmanagerwindow.hpp"

FooSettingsManagerWindow::FooSettingsManagerWindow(QWidget *parent) : QSplitter(parent)
{
	//splitter = new QSplitter(this);
	//splitter->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	setChildrenCollapsible(false);

	treeWidget = new QTreeWidget(this);
	treeWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
	treeWidget->setHeaderHidden(true);
	addWidget(treeWidget);

	layout = new QGridLayout(this);
	widget = new QWidget(this);
	widget->setLayout(layout);
	addWidget(widget);

	cancelButton = new QPushButton(tr("&Cancel"),this);
	cancelButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	spacer = new QSpacerItem(200, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
	okButton = new QPushButton(tr("&OK"), this);
	applyButton = new QPushButton(tr("&Apply"), this);

	layout->addWidget(cancelButton, 1, 0, 1, 1, Qt::AlignBottom);
	layout->addItem(spacer, 1, 1, 1, 1);
	layout->addWidget(okButton, 1, 2, 1, 1, Qt::AlignBottom);
	layout->addWidget(applyButton, 1, 3, 1, 1, Qt::AlignBottom);
}
