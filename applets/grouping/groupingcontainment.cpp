/*
 *   SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>
 *   SPDX-FileCopyrightText: 2016 David Edmundson <davidedmundson@kde.org>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "groupingcontainment.h"
#include "debug.h"

#include <QDebug>
#include <QProcess>

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusPendingCallWatcher>
#include <QMenu>
#include <QQuickItem>
#include <QQuickWindow>
#include <QRegExp>
#include <QScreen>
#include <QStandardItemModel>

#include <Plasma/PluginLoader>
#include <Plasma/ServiceJob>

#include <KActionCollection>
#include <KLocalizedString>

#include <plasma_version.h>

GroupingContainment::GroupingContainment(QObject *parent, const QVariantList &args)
    : Plasma::Containment(parent, args)
{
    setHasConfigurationInterface(true);
    setContainmentType(Plasma::Types::CustomEmbeddedContainment);
}

GroupingContainment::~GroupingContainment()
{
}

void GroupingContainment::init()
{
    Containment::init();
}

void GroupingContainment::newTask(const QString &task)
{
    createApplet(task, QVariantList() << QStringLiteral("org.kde.plasma:force-create"));
}

void GroupingContainment::cleanupTask(const QString &task)
{
    foreach (Plasma::Applet *applet, applets()) {
        if (!applet->pluginMetaData().isValid() || task == applet->pluginMetaData().pluginId()) {
            applet->destroy();
        }
    }
}

void GroupingContainment::showPlasmoidMenu(QQuickItem *appletInterface, int x, int y)
{
    if (!appletInterface) {
        return;
    }

    Plasma::Applet *applet = appletInterface->property("_plasma_applet").value<Plasma::Applet *>();

    QPointF pos = appletInterface->mapToScene(QPointF(x, y));

    if (appletInterface->window() && appletInterface->window()->screen()) {
        pos = appletInterface->window()->mapToGlobal(pos.toPoint());
    } else {
        pos = QPoint();
    }

    QMenu *desktopMenu = new QMenu;
    connect(this, &QObject::destroyed, desktopMenu, &QMenu::close);
    desktopMenu->setAttribute(Qt::WA_DeleteOnClose);

    emit applet->contextualActionsAboutToShow();
    foreach (QAction *action, applet->contextualActions()) {
        if (action) {
            desktopMenu->addAction(action);
        }
    }

    // add run associated action/ remove / alternatives
    desktopMenu->addActions(applet->actions()->actions());

    if (desktopMenu->isEmpty()) {
        delete desktopMenu;
        return;
    }

    desktopMenu->adjustSize();

    if (QScreen *screen = appletInterface->window()->screen()) {
        const QRect geo = screen->availableGeometry();

        pos =
            QPoint(qBound(geo.left(), (int)pos.x(), geo.right() - desktopMenu->width()), qBound(geo.top(), (int)pos.y(), geo.bottom() - desktopMenu->height()));
    }

    desktopMenu->popup(pos.toPoint());
}

QPointF GroupingContainment::popupPosition(QQuickItem *visualParent, int x, int y)
{
    if (!visualParent) {
        return QPointF(0, 0);
    }

    QPointF pos = visualParent->mapToScene(QPointF(x, y));

    if (visualParent->window() && visualParent->window()->screen()) {
        pos = visualParent->window()->mapToGlobal(pos.toPoint());
    } else {
        return QPoint();
    }
    return pos;
}

void GroupingContainment::reorderItemBefore(QQuickItem *before, QQuickItem *after)
{
    if (!before || !after) {
        return;
    }

    before->setVisible(false);
    before->setParentItem(after->parentItem());
    before->stackBefore(after);
    before->setVisible(true);
}

void GroupingContainment::reorderItemAfter(QQuickItem *after, QQuickItem *before)
{
    if (!before || !after) {
        return;
    }

    after->setVisible(false);
    after->setParentItem(before->parentItem());
    after->stackAfter(before);
    after->setVisible(true);
}

K_EXPORT_PLASMA_APPLET_WITH_JSON(groupingcontainment, GroupingContainment, "metadata.json")

#include "groupingcontainment.moc"
