// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QTest>

#include <App/Application.h>
#include <Gui/Widgets.h>
#include <src/App/InitApplication.h>

class testStatefulLabel: public QObject
{
    Q_OBJECT

public:
    testStatefulLabel()
    {
        tests::initApplication();
    }

private Q_SLOTS:
    void preferenceChangesBeforeFirstState()
    {
        auto group = App::GetApplication().GetParameterGroupByPath(
            "User parameter:BaseApp/Preferences/Tests/StatefulLabel"
        );
        Gui::StatefulLabel label;
        label.setParameterGroup("User parameter:BaseApp/Preferences/Tests/StatefulLabel");
        label.registerState(QStringLiteral("error"), QColor(Qt::red), std::string("ErrorColor"));

        constexpr unsigned long green = 0x00ff00ff;
        group->SetUnsigned("ErrorColor", green);
        label.setState(QStringLiteral("error"));
        QVERIFY(label.styleSheet().contains(QStringLiteral("rgba(0,255,0,255)")));
    }

    void preferenceChangesAfterStylesheetClearsCache()
    {
        auto group = App::GetApplication().GetParameterGroupByPath(
            "User parameter:BaseApp/Preferences/Tests/StatefulLabel"
        );
        auto general = App::GetApplication().GetParameterGroupByPath(
            "User parameter:BaseApp/Preferences/General"
        );
        Gui::StatefulLabel label;
        label.setParameterGroup("User parameter:BaseApp/Preferences/Tests/StatefulLabel");
        label.registerState(QStringLiteral("error"), QColor(Qt::red), std::string("ErrorColor"));
        label.setState(QStringLiteral("error"));
        label.OnChange(*general, "StyleSheet");

        constexpr unsigned long blue = 0x0000ffff;
        group->SetUnsigned("ErrorColor", blue);
        label.setState(QStringLiteral("error"));
        QVERIFY(label.styleSheet().contains(QStringLiteral("rgba(0,0,255,255)")));
    }
};

QTEST_MAIN(testStatefulLabel)

#include "StatefulLabel.moc"
