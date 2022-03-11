/*
 *   SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>
 *
 *   SPDX-License-Identifier: LGPL-2.0-only
 */

#ifndef COMIC_DATAENGINE_H
#define COMIC_DATAENGINE_H

#include <Plasma/DataEngine>
// Qt
#include <QIcon>
#include <QNetworkConfigurationManager>

class ComicProvider;

struct ComicProviderInfo {
    QString pluginId;
    QString name;
    QString icon;
};

/**
 * This class provides the comic strip.
 *
 * The query keys have the following structure:
 *   \<comic_identifier\>:\<suffix\>
 * usually the suffix is the date
 * e.g.
 *   userfriendly:2007-07-19
 * but some other comics uses numerical identifiers, like
 *   xkcd:378
 * if the suffix is empty the latest comic will be returned
 *
 */
class ComicEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    ComicEngine();
    ~ComicEngine() override;

    QList<ComicProviderInfo> loadProviders();

protected:
    void init();
    bool sourceRequestEvent(const QString &identifier) override;

protected Q_SLOTS:
    bool updateSourceEvent(const QString &identifier) override;

private Q_SLOTS:
    void finished(ComicProvider *);
    void error(ComicProvider *);
    void onOnlineStateChanged(bool);

private:
    bool mEmptySuffix;
    void setComicData(ComicProvider *provider);
    QString lastCachedIdentifier(const QString &identifier) const;
    QString mIdentifierError;
    QHash<QString, ComicProvider *> m_jobs;
    QNetworkConfigurationManager m_networkConfigurationManager;
    QSet<QString> mProviders;
};

#endif
