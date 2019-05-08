///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef PLUGINS_SAMPLESOURCE_PERSEUS_PERSEUSINPUT_H_
#define PLUGINS_SAMPLESOURCE_PERSEUS_PERSEUSINPUT_H_

#include <vector>

#include <QNetworkRequest>

#include "perseus-sdr.h"
#include "dsp/devicesamplesource.h"
#include "util/message.h"
#include "perseussettings.h"

class QNetworkAccessManager;
class QNetworkReply;
class DeviceAPI;
class FileRecord;
class PerseusThread;

class PerseusInput : public DeviceSampleSource {
    Q_OBJECT
public:
    class MsgConfigurePerseus : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        const PerseusSettings& getSettings() const { return m_settings; }
        bool getForce() const { return m_force; }

        static MsgConfigurePerseus* create(const PerseusSettings& settings, bool force)
        {
            return new MsgConfigurePerseus(settings, force);
        }

    private:
        PerseusSettings m_settings;
        bool m_force;

        MsgConfigurePerseus(const PerseusSettings& settings, bool force) :
            Message(),
            m_settings(settings),
            m_force(force)
        { }
    };

    class MsgFileRecord : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        bool getStartStop() const { return m_startStop; }

        static MsgFileRecord* create(bool startStop) {
            return new MsgFileRecord(startStop);
        }

    protected:
        bool m_startStop;

        MsgFileRecord(bool startStop) :
            Message(),
            m_startStop(startStop)
        { }
    };

    class MsgStartStop : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        bool getStartStop() const { return m_startStop; }

        static MsgStartStop* create(bool startStop) {
            return new MsgStartStop(startStop);
        }

    protected:
        bool m_startStop;

        MsgStartStop(bool startStop) :
            Message(),
            m_startStop(startStop)
        { }
    };

    PerseusInput(DeviceAPI *deviceAPI);
    ~PerseusInput();
    virtual void destroy();

    virtual void init();
    virtual bool start();
    virtual void stop();

    virtual QByteArray serialize() const;
    virtual bool deserialize(const QByteArray& data);

    virtual void setMessageQueueToGUI(MessageQueue *queue) { m_guiMessageQueue = queue; }
    virtual const QString& getDeviceDescription() const;
    virtual int getSampleRate() const;
    virtual void setSampleRate(int sampleRate) { (void) sampleRate; }
    virtual quint64 getCenterFrequency() const;
    virtual void setCenterFrequency(qint64 centerFrequency);

    virtual bool handleMessage(const Message& message);

    virtual int webapiSettingsGet(
                SWGSDRangel::SWGDeviceSettings& response,
                QString& errorMessage);

    virtual int webapiSettingsPutPatch(
                bool force,
                const QStringList& deviceSettingsKeys,
                SWGSDRangel::SWGDeviceSettings& response, // query + response
                QString& errorMessage);

    virtual int webapiReportGet(
            SWGSDRangel::SWGDeviceReport& response,
            QString& errorMessage);

    virtual int webapiRunGet(
            SWGSDRangel::SWGDeviceState& response,
            QString& errorMessage);

    virtual int webapiRun(
            bool run,
            SWGSDRangel::SWGDeviceState& response,
            QString& errorMessage);

    const std::vector<uint32_t>& getSampleRates() const { return m_sampleRates; }

private:
    DeviceAPI *m_deviceAPI;
    FileRecord *m_fileSink;
    QString m_deviceDescription;
    PerseusSettings m_settings;
    bool m_running;
    PerseusThread *m_perseusThread;
    perseus_descr *m_perseusDescriptor;
    std::vector<uint32_t> m_sampleRates;
    QNetworkAccessManager *m_networkManager;
    QNetworkRequest m_networkRequest;

    bool openDevice();
    void closeDevice();
    void setDeviceCenterFrequency(quint64 freq, const PerseusSettings& settings);
    bool applySettings(const PerseusSettings& settings, bool force = false);
    void webapiFormatDeviceSettings(SWGSDRangel::SWGDeviceSettings& response, const PerseusSettings& settings);
    void webapiFormatDeviceReport(SWGSDRangel::SWGDeviceReport& response);
    void webapiReverseSendSettings(QList<QString>& deviceSettingsKeys, const PerseusSettings& settings, bool force);
    void webapiReverseSendStartStop(bool start);

private slots:
    void networkManagerFinished(QNetworkReply *reply);
};

#endif /* PLUGINS_SAMPLESOURCE_PERSEUS_PERSEUSINPUT_H_ */
