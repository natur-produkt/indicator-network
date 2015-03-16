/*
 * Copyright (C) 2014 Canonical, Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *     Antti Kaijanmäki <antti.kaijanmaki@canonical.com>
 */

#ifndef MODEM_H
#define MODEM_H

#include <map>
#include <memory>
#include <string>

#include <QString>
#include <QObject>

class QOfonoModem;

/**
 * all signals and property changes emitted from GMainLoop
 */
class Modem: public QObject, public std::enable_shared_from_this<Modem>
{
    Q_OBJECT

    class Private;
    std::shared_ptr<Private> d;

public:
    enum class PinType
    {
        none,
        pin,
        puk
    };

    enum class SimStatus
    {
        missing,
        error,
        locked,
        permanentlyLocked,
        ready,
        not_available
    };

    enum class Status
    {
        unregistered,
        registered,
        searching,
        denied,
        unknown,
        roaming
    };

    enum class Technology
    {
        notAvailable,
        gsm,
        edge,
        umts,
        hspa,
        lte
    };

    typedef std::shared_ptr<Modem> Ptr;
    typedef std::weak_ptr<Modem> WeakPtr;

    struct Compare
    {
        bool operator()(int lhs, int rhs)
        {
            if (lhs == -1 && rhs == -1)
                return false;
            if (lhs == -1)
                return false;
            if (rhs == -1)
                return true;
            return lhs < rhs;
        }
    };

    Modem() = delete;

    explicit Modem(std::shared_ptr<QOfonoModem> ofonoModem);

    virtual ~Modem();

//    org::ofono::Interface::Modem::Ptr ofonoModem() const;

    void enterPin(PinType type,
                  const QString &pin);

    void resetPin(PinType type,
                  const QString &puk,
                  const QString &pin);

    void changePin(PinType type,
                   const QString &oldPin,
                   const QString &newPin);

    Q_PROPERTY(bool online READ online NOTIFY onlineUpdated)
    bool online() const;

    Q_PROPERTY(Modem::SimStatus simStatus READ simStatus NOTIFY simStatusUpdated)
    SimStatus simStatus() const;

    Q_PROPERTY(Modem::PinType requiredPin READ requiredPin NOTIFY requiredPinUpdated)
    PinType requiredPin() const;

//    Q_PROPERTY(bool retries READ retries NOTIFY retriesUpdated)
    const std::map<PinType, std::uint8_t> &retries() const;

    Q_PROPERTY(QString operatorName READ operatorName NOTIFY operatorNameUpdated)
    const QString &operatorName() const;

    Q_PROPERTY(Modem::Status status READ status NOTIFY statusUpdated)
    Status status() const;

    Q_PROPERTY(std::int8_t strength READ strength NOTIFY strengthUpdated)
    std::int8_t strength() const;

    Q_PROPERTY(Modem::Technology technology READ technology NOTIFY technologyUpdated)
    Technology technology() const;

    Q_PROPERTY(bool dataEnabled READ dataEnabled NOTIFY dataEnabledUpdated)
    bool dataEnabled() const;

    Q_PROPERTY(QString simIdentifier READ simIdentifier NOTIFY simIdentifierUpdated)
    const QString &simIdentifier() const;

    int index() const;

    QString name() const;

    static QString strengthIcon(int8_t strength);

    static QString technologyIcon(Technology tech);

Q_SIGNALS:
    void onlineUpdated(bool);

    void simStatusUpdated(SimStatus);

    void requiredPinUpdated(PinType);

    void retriesUpdated();

    void operatorNameUpdated(const QString &);

    void statusUpdated(Status);

    void strengthUpdated(std::int8_t);

    void technologyUpdated(Technology);

    void dataEnabledUpdated(bool);

    void simIdentifierUpdated(const QString &);

    void updated(Modem::Ptr);
};

#endif
