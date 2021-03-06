/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "platformsettings.h"
#include <QDir>
#include <QSettings>
#include <Quill>
#if defined(QT4)
#include <QDeclarativeInfo>
#elif defined(QT5)
#include <QQmlInfo>
#endif
#include "quillitem.h"

#ifdef HARMATTAN
#define PATH "/usr/share/cameraplus/config/cameraplus.ini"
#else
#define PATH "/usr/share/harbour-cameraplus/share/cameraplus/config/cameraplus.ini"
#endif

#define PREVIEW_SIZE                             QSize(854, 480)
#define PREVIEW_FLAVOR_NAME                      QString("screen")
#define GRID_SIZE                                QSize(170, 170)
#define GRID_FLAVOR_NAME                         QString("cropped")
#define TEMPORARY_FILE_PATH                      "%1%2.config%2quill%2tmp"
#define THUMBNAIL_EXTENSION                      QString("jpeg")
#define THUMBNAIL_CREATION_ENABLED               true
#define DBUS_THUMBNAILING_ENABLED                true
#define BACKGROUND_RENDERING_COLOR               QColor(Qt::black)
#define MAX_TEXTURE_SIZE                         QSize(2048, 2048)

#ifdef HARMATTAN
#define IMAGE_PATH QString("%1%2MyDocs%2DCIM%2").arg(QDir::homePath()).arg(QDir::separator())
#define VIDEO_PATH IMAGE_PATH
#define TEMP_VIDEO QString("%1%2MyDocs%2.cameraplus%2").arg(QDir::homePath()).arg(QDir::separator())
#endif

#ifdef SAILFISH
#define IMAGE_PATH QString("%1%2Pictures%2Camera%2").arg(QDir::homePath()).arg(QDir::separator())
#define VIDEO_PATH QString("%1%2Videos%2Camera%2").arg(QDir::homePath()).arg(QDir::separator())
#define TEMP_VIDEO VIDEO_PATH
#endif

PlatformSettings::PlatformSettings(QObject *parent) :
  QObject(parent), m_settings(new QSettings(PATH, QSettings::IniFormat)) {

}

PlatformSettings::~PlatformSettings() {
  delete m_settings;
}

QSize PlatformSettings::previewSize() {
  return m_settings->value("quill/previewSize", PREVIEW_SIZE).toSize();
}

QString PlatformSettings::previewFlavorName() {
  return m_settings->value("quill/previewFlavorName", PREVIEW_FLAVOR_NAME).toString();
}

QSize PlatformSettings::gridSize() {
  return m_settings->value("quill/gridSize", GRID_SIZE).toSize();
}

QString PlatformSettings::gridFlavorName() {
  return m_settings->value("quill/gridFlavorName", GRID_FLAVOR_NAME).toString();
}

QString PlatformSettings::thumbnailExtension() {
  return m_settings->value("quill/thumbnailExtension", THUMBNAIL_EXTENSION).toString();
}

QColor PlatformSettings::backgroundRenderingColor() {
  return m_settings->value("quill/backgroundRenderingColor",
			   BACKGROUND_RENDERING_COLOR).value<QColor>();
}

bool PlatformSettings::isDBusThumbnailingEnabled() {
  return m_settings->value("quill/dbusThumbnailingEnabled", DBUS_THUMBNAILING_ENABLED).toBool();
}

bool PlatformSettings::isThumbnailCreationEnabled() {
  return m_settings->value("quill/thumbnailCreationEnabled", THUMBNAIL_CREATION_ENABLED).toBool();
}

QString PlatformSettings::temporaryFilePath() {
  QString defaultPath = QString(TEMPORARY_FILE_PATH).arg(QDir::homePath()).arg(QDir::separator());
  return m_settings->value("quill/temporaryFilePath", defaultPath).toString();
}

void PlatformSettings::init() {
  Quill::setPreviewLevelCount(3);

  {
    // cropped display level
    QSize size = gridSize();
    int len = qMax(size.width(), size.height());
    Quill::setThumbnailFlavorName(QuillItem::DisplayLevelCropped, gridFlavorName());
    Quill::setPreviewSize(QuillItem::DisplayLevelCropped, QSize(len, len));
    Quill::setMinimumPreviewSize(QuillItem::DisplayLevelCropped, QSize(len, len));
  }

  {
    // full screen display level
    QSize size = previewSize();
    int len = qMax(size.width(), size.height());

    Quill::setThumbnailFlavorName(QuillItem::DisplayLevelFullScreen, previewFlavorName());
    Quill::setPreviewSize(QuillItem::DisplayLevelFullScreen, QSize(len, len));
  }

  // large display level
  Quill::setPreviewSize(QuillItem::DisplayLevelLarge, MAX_TEXTURE_SIZE);

  Quill::setThumbnailExtension(thumbnailExtension());
  Quill::setBackgroundRenderingColor(backgroundRenderingColor());
  Quill::setDBusThumbnailingEnabled(isDBusThumbnailingEnabled());
  Quill::setThumbnailCreationEnabled(isThumbnailCreationEnabled());

  QString tempPath = temporaryFilePath();
  QDir().mkpath(tempPath);
  Quill::setTemporaryFilePath(tempPath);
}

PlatformSettings::Service PlatformSettings::service(const QString& id) {
  PlatformSettings::Service service;
  m_settings->beginGroup(id);

  service.m_enabled = m_settings->value("enabled", false).toBool();
  service.m_type = m_settings->value("type").toString();
  service.m_path = m_settings->value("path").toString();
  service.m_interface = m_settings->value("interface").toString();
  service.m_dest = m_settings->value("dest").toString();
  service.m_method = m_settings->value("method").toString();

  m_settings->endGroup();

  return service;
}

QString PlatformSettings::imageCaptureStartedSound() const {
  return m_settings->value("sounds/imageCaptureStarted").toString();
}

QString PlatformSettings::imageCaptureEndedSound() const {
  return m_settings->value("sounds/imageCaptureEnded").toString();
}

QString PlatformSettings::videoRecordingStartedSound() const {
  return m_settings->value("sounds/videoRecordingStarted").toString();
}

QString PlatformSettings::videoRecordingEndedSound() const {
  return m_settings->value("sounds/videoRecordingEnded").toString();
}

QString PlatformSettings::autoFocusAcquiredSound() const {
  return m_settings->value("sounds/autoFocusAcquired").toString();
}

QString PlatformSettings::autoFocusFailedSound() const {
  return m_settings->value("sounds/autoFocusFailed").toString();
}

QString PlatformSettings::imagePath() {
  if (m_image.isEmpty()) {
    m_image = canonicalPath(IMAGE_PATH);
  }

  return m_image;
}

QString PlatformSettings::videoPath() {
  if (m_video.isEmpty()) {
    m_video = canonicalPath(VIDEO_PATH);
  }

  return m_video;
}

QString PlatformSettings::temporaryVideoPath() {
  if (m_temp.isEmpty()) {
    m_temp = canonicalPath(TEMP_VIDEO);
  }

  return m_temp;
}

QString PlatformSettings::canonicalPath(const QString& path) const {
  if (!QDir::root().mkpath(path)) {
    qmlInfo(this) << "Failed to create path" << path;
    return QString();
  }

  QString newPath = QFileInfo(path).canonicalFilePath();

  if (newPath.isEmpty()) {
    return newPath;
  }

  if (!newPath.endsWith(QDir::separator())) {
    newPath.append(QDir::separator());
  }

  return newPath;
}

int PlatformSettings::maximumVideoDuration() const {
#ifdef SAILFISH
  return -1;
#else
  return 3600;
#endif
}

int PlatformSettings::naturalOrientationIsLandscape() const {
#ifdef SAILFISH
  return false;
#else
  return true;
#endif
}

int PlatformSettings::primarySensorOrientationAngle() const {
#ifdef SAILFISH
  return -1;
#else
  return 0;
#endif
}

int PlatformSettings::secondarySensorOrientationAngle() const {
#ifdef SAILFISH
  return -1;
#else
  return 0;
#endif
}
