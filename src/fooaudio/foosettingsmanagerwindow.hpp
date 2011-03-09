#ifndef FOOSETTINGSMANAGERWINDOW_HPP
#define FOOSETTINGSMANAGERWINDOW_HPP

#include <QWidget>
#include <QSplitter>
#include <QTreeWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QSpacerItem>

namespace Fooaudio
{
	class FooSettingsManagerWindow : public QSplitter
	{
		Q_OBJECT

	public:
		explicit FooSettingsManagerWindow(QWidget *parent = 0);

	private:
		QTreeWidget *treeWidget;
		QWidget *widget;
		QGridLayout *layout;
		QSpacerItem *spacer;
		QPushButton *okButton;
		QPushButton *cancelButton;
		QPushButton *applyButton;

	signals:

	public slots:

	};
}

#endif // FOOSETTINGSMANAGERWINDOW_HPP
