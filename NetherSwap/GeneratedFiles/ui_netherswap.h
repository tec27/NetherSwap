/********************************************************************************
** Form generated from reading UI file 'netherswap.ui'
**
** Created: Mon Jan 7 23:24:27 2013
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETHERSWAP_H
#define UI_NETHERSWAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "hotkey_button.h"

QT_BEGIN_NAMESPACE

class Ui_NetherSwapClass
{
public:
    QWidget *central_widget;
    QVBoxLayout *verticalLayout;
    QFrame *title_bar;
    QHBoxLayout *horizontalLayout;
    QLabel *app_icon;
    QLabel *app_title;
    QSpacerItem *horizontalSpacer;
    QPushButton *minimize_btn;
    QPushButton *close_btn;
    QFrame *content;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *hotkeys_box;
    QFormLayout *formLayout;
    QLabel *in_game_label;
    QLabel *out_of_game_label;
    HotkeyButton *in_game_hotkey_btn;
    HotkeyButton *out_of_game_hotkey_btn;
    QLabel *status_label;
    QFrame *footer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *version_label;
    QSpacerItem *horizontalSpacer_3;
    QLabel *author_label;

    void setupUi(QMainWindow *NetherSwapClass)
    {
        if (NetherSwapClass->objectName().isEmpty())
            NetherSwapClass->setObjectName(QStringLiteral("NetherSwapClass"));
        NetherSwapClass->resize(380, 180);
        QFont font;
        font.setFamily(QStringLiteral("Segoe UI"));
        font.setPointSize(9);
        NetherSwapClass->setFont(font);
        QIcon icon;
        icon.addFile(QStringLiteral(":/NetherSwap/Resources/icons/netherswap-tray.ico"), QSize(), QIcon::Normal, QIcon::Off);
        NetherSwapClass->setWindowIcon(icon);
        NetherSwapClass->setStyleSheet(QLatin1String("#central_widget {\n"
"	border-top: 1px solid #333333;\n"
"	border-bottom: 1px solid #cccccc;\n"
"	border-left: 1px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"		stop: 0 #333333, stop: 1 #cccccc);\n"
"	border-right: 1px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"		stop: 0 #333333, stop: 1 #cccccc);\n"
"}\n"
"\n"
"#title_bar {\n"
"	border: none;\n"
"	background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"		stop: 0 #868a8f, stop: 0.08 #5f6468,\n"
"		stop: 0.49999 #31363a, stop: 0.5 #21262a,\n"
"		stop: 0.9 #061011, stop: 1 #040a0f);\n"
"}\n"
"\n"
"#app_title {\n"
"	color: #efefef;\n"
"	margin-left: 2px;\n"
"}\n"
"\n"
"#title_bar QPushButton {\n"
"	color: #333;\n"
"	border: 1px solid #555;\n"
"	border-radius: 2px;\n"
"	padding: 0px;\n"
"	background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"		fx: 0.3, fy: -0.4,\n"
"		radius: 1.35, stop: 0 #fff, stop: 1 #888);\n"
"	min-width: 24px;\n"
"}\n"
"\n"
"#title_bar QPushButton:hover {\n"
"	background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"		fx: 0.3,"
                        " fy: -0.4,\n"
"		radius: 1.35, stop: 0 #fff, stop: 1 #b0b4b8);\n"
"}\n"
"\n"
"#title_bar QPushButton:pressed {\n"
"	background: qradialgradient(cx: 0.4, cy: -0.1,\n"
"		fx: 0.4, fy: -0.1,\n"
"		radius: 1.35, stop: 0 #fff, stop: 1 #d0d4d8);\n"
"}\n"
"\n"
"#content {\n"
"	border: none;\n"
"	background: #21262a;\n"
"}\n"
"\n"
"#footer {\n"
"	border: none;\n"
"	background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"		stop: 0 #666a6f, stop: 0.08 #3f4448,\n"
"		stop: 0.39999 #31363a, stop: 0.4 #21262a,\n"
"		stop: 0.9 #061011, stop: 1 #040a0f);\n"
"}\n"
"\n"
"#footer QLabel {\n"
"	color: #f5f5f5;\n"
"}\n"
"\n"
"#hotkeys_box {\n"
"	color: #f5f5f5;\n"
"	margin-top: 6px;\n"
"	border: 2px solid #5f6468;\n"
"	border-radius: 5px;\n"
"}\n"
"\n"
"#hotkeys_box::title {\n"
"	subcontrol-origin: border;\n"
"	left: 6px;\n"
"	top: -8px;\n"
"}\n"
"\n"
"#hotkeys_box QLabel {\n"
"	color: #f5f5f5;\n"
"}\n"
"\n"
"#hotkeys_box QPushButton {\n"
"	color: #333;\n"
"	border: 1px solid #555;\n"
"	border-radius: 2px;\n"
"	padding: 0px;\n"
""
                        "	background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"		fx: 0.3, fy: -0.4,\n"
"		radius: 1.35, stop: 0 #fff, stop: 1 #888);\n"
"	min-width: 24px;\n"
"}\n"
"\n"
"#hotkeys_box QPushButton:hover {\n"
"	background: qradialgradient(cx: 0.3, cy: -0.4,\n"
"		fx: 0.3, fy: -0.4,\n"
"		radius: 1.35, stop: 0 #fff, stop: 1 #b0b4b8);\n"
"}\n"
"\n"
"#hotkeys_box QPushButton:pressed {\n"
"	background: qradialgradient(cx: 0.4, cy: -0.1,\n"
"		fx: 0.4, fy: -0.1,\n"
"		radius: 1.35, stop: 0 #fff, stop: 1 #d0d4d8);\n"
"}\n"
"#status_label {\n"
"	color: #f5f5f5;\n"
"}"));
        central_widget = new QWidget(NetherSwapClass);
        central_widget->setObjectName(QStringLiteral("central_widget"));
        verticalLayout = new QVBoxLayout(central_widget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        title_bar = new QFrame(central_widget);
        title_bar->setObjectName(QStringLiteral("title_bar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(title_bar->sizePolicy().hasHeightForWidth());
        title_bar->setSizePolicy(sizePolicy);
        title_bar->setFrameShape(QFrame::StyledPanel);
        title_bar->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(title_bar);
        horizontalLayout->setSpacing(2);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(4, 0, 4, 0);
        app_icon = new QLabel(title_bar);
        app_icon->setObjectName(QStringLiteral("app_icon"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(app_icon->sizePolicy().hasHeightForWidth());
        app_icon->setSizePolicy(sizePolicy1);
        app_icon->setMinimumSize(QSize(32, 32));
        app_icon->setMaximumSize(QSize(32, 32));
        app_icon->setPixmap(QPixmap(QString::fromUtf8(":/NetherSwap/Resources/icons/netherswap-tray.ico")));
        app_icon->setScaledContents(true);

        horizontalLayout->addWidget(app_icon);

        app_title = new QLabel(title_bar);
        app_title->setObjectName(QStringLiteral("app_title"));
        QFont font1;
        font1.setFamily(QStringLiteral("Segoe UI"));
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        app_title->setFont(font1);
        app_title->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(app_title);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        minimize_btn = new QPushButton(title_bar);
        minimize_btn->setObjectName(QStringLiteral("minimize_btn"));
        sizePolicy1.setHeightForWidth(minimize_btn->sizePolicy().hasHeightForWidth());
        minimize_btn->setSizePolicy(sizePolicy1);
        minimize_btn->setMinimumSize(QSize(26, 24));
        minimize_btn->setMaximumSize(QSize(24, 24));
        QFont font2;
        font2.setFamily(QStringLiteral("Segoe UI"));
        font2.setPointSize(15);
        font2.setBold(true);
        font2.setWeight(75);
        minimize_btn->setFont(font2);
        minimize_btn->setFlat(true);

        horizontalLayout->addWidget(minimize_btn);

        close_btn = new QPushButton(title_bar);
        close_btn->setObjectName(QStringLiteral("close_btn"));
        sizePolicy1.setHeightForWidth(close_btn->sizePolicy().hasHeightForWidth());
        close_btn->setSizePolicy(sizePolicy1);
        close_btn->setMinimumSize(QSize(26, 24));
        close_btn->setMaximumSize(QSize(24, 24));
        QFont font3;
        font3.setFamily(QStringLiteral("Segoe UI"));
        font3.setPointSize(11);
        font3.setBold(true);
        font3.setWeight(75);
        close_btn->setFont(font3);
        close_btn->setText(QStringLiteral("X"));
        close_btn->setFlat(true);

        horizontalLayout->addWidget(close_btn);


        verticalLayout->addWidget(title_bar);

        content = new QFrame(central_widget);
        content->setObjectName(QStringLiteral("content"));
        content->setFont(font);
        content->setFrameShape(QFrame::StyledPanel);
        content->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(content);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        hotkeys_box = new QGroupBox(content);
        hotkeys_box->setObjectName(QStringLiteral("hotkeys_box"));
        hotkeys_box->setFont(font);
        formLayout = new QFormLayout(hotkeys_box);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setVerticalSpacing(12);
        formLayout->setContentsMargins(4, -1, 4, -1);
        in_game_label = new QLabel(hotkeys_box);
        in_game_label->setObjectName(QStringLiteral("in_game_label"));
        QFont font4;
        font4.setFamily(QStringLiteral("Segoe UI"));
        font4.setPointSize(10);
        in_game_label->setFont(font4);

        formLayout->setWidget(0, QFormLayout::LabelRole, in_game_label);

        out_of_game_label = new QLabel(hotkeys_box);
        out_of_game_label->setObjectName(QStringLiteral("out_of_game_label"));
        out_of_game_label->setFont(font4);

        formLayout->setWidget(1, QFormLayout::LabelRole, out_of_game_label);

        in_game_hotkey_btn = new HotkeyButton(hotkeys_box);
        in_game_hotkey_btn->setObjectName(QStringLiteral("in_game_hotkey_btn"));
        in_game_hotkey_btn->setFont(font4);

        formLayout->setWidget(0, QFormLayout::FieldRole, in_game_hotkey_btn);

        out_of_game_hotkey_btn = new HotkeyButton(hotkeys_box);
        out_of_game_hotkey_btn->setObjectName(QStringLiteral("out_of_game_hotkey_btn"));
        out_of_game_hotkey_btn->setFont(font4);

        formLayout->setWidget(1, QFormLayout::FieldRole, out_of_game_hotkey_btn);


        verticalLayout_2->addWidget(hotkeys_box);

        status_label = new QLabel(content);
        status_label->setObjectName(QStringLiteral("status_label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(status_label->sizePolicy().hasHeightForWidth());
        status_label->setSizePolicy(sizePolicy2);
        QFont font5;
        font5.setFamily(QStringLiteral("Segoe UI"));
        font5.setPointSize(11);
        status_label->setFont(font5);
        status_label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(status_label);


        verticalLayout->addWidget(content);

        footer = new QFrame(central_widget);
        footer->setObjectName(QStringLiteral("footer"));
        sizePolicy.setHeightForWidth(footer->sizePolicy().hasHeightForWidth());
        footer->setSizePolicy(sizePolicy);
        footer->setFrameShape(QFrame::StyledPanel);
        footer->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(footer);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(3, 0, 3, 0);
        version_label = new QLabel(footer);
        version_label->setObjectName(QStringLiteral("version_label"));
        QFont font6;
        font6.setFamily(QStringLiteral("Segoe UI"));
        font6.setPointSize(9);
        font6.setBold(false);
        font6.setWeight(50);
        version_label->setFont(font6);

        horizontalLayout_2->addWidget(version_label);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        author_label = new QLabel(footer);
        author_label->setObjectName(QStringLiteral("author_label"));
        author_label->setFont(font);

        horizontalLayout_2->addWidget(author_label);


        verticalLayout->addWidget(footer);

        NetherSwapClass->setCentralWidget(central_widget);

        retranslateUi(NetherSwapClass);

        QMetaObject::connectSlotsByName(NetherSwapClass);
    } // setupUi

    void retranslateUi(QMainWindow *NetherSwapClass)
    {
        NetherSwapClass->setWindowTitle(QApplication::translate("NetherSwapClass", "NetherSwap", 0));
        app_icon->setText(QString());
        app_title->setText(QApplication::translate("NetherSwapClass", "NetherSwap", 0));
        minimize_btn->setText(QApplication::translate("NetherSwapClass", "\342\200\223", 0));
        hotkeys_box->setTitle(QApplication::translate("NetherSwapClass", "Hotkeys", 0));
        in_game_label->setText(QApplication::translate("NetherSwapClass", "Game Scene", 0));
        out_of_game_label->setText(QApplication::translate("NetherSwapClass", "Background/Menu Scene", 0));
        in_game_hotkey_btn->setText(QApplication::translate("NetherSwapClass", "Not Set", 0));
        out_of_game_hotkey_btn->setText(QApplication::translate("NetherSwapClass", "Not Set", 0));
        status_label->setText(QApplication::translate("NetherSwapClass", "Initializing...", 0));
        version_label->setText(QApplication::translate("NetherSwapClass", "v1.0", 0));
        author_label->setText(QApplication::translate("NetherSwapClass", "by tec27", 0));
    } // retranslateUi

};

namespace Ui {
    class NetherSwapClass: public Ui_NetherSwapClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETHERSWAP_H
