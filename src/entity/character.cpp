#include "character.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QHash>
#include <QPixmapCache>
#include <QStringList>


QStringList possibleAssetRoots()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString curDir = QDir::currentPath();

    QStringList roots;

    roots << appDir + "/assets";
    roots << appDir + "/../assets";
    roots << appDir + "/../../assets";
    roots << appDir + "/../../../assets";
    roots << appDir + "/../../../../assets";

    roots << curDir + "/assets";
    roots << curDir + "/../assets";
    roots << curDir + "/../../assets";
    roots << curDir + "/../../../assets";

    return roots;
}

QString subFolderByRole(const QString &roleName)
{
    if (roleName == "player" || roleName.startsWith("player"))
    {
        return "player";
    }

    return "enemy";
}

QString findAssetFile(const QString &subFolder, const QString &fileName)
{
    QStringList roots = possibleAssetRoots();

    for (const QString &root : roots)
    {
        QString path = QDir(root).filePath(subFolder + "/" + fileName);

        if (QFileInfo::exists(path))
        {
            return path;
        }
    }

    return QString();
}

bool loadOnePixmap(QPixmap &pix, const QString &subFolder,
                   const QString &fileName, const QSize &targetSize)
{
    QString path = findAssetFile(subFolder, fileName);

    if (path.isEmpty())
    {
        return false;
    }

    QString cacheKey = path + QString("@%1x%2")
                                  .arg(targetSize.width())
                                  .arg(targetSize.height());

    if (QPixmapCache::find(cacheKey, &pix))
    {
        return true;
    }

    QPixmap raw;

    if (!raw.load(path))
    {
        return false;
    }

    if (targetSize.width() > 0 && targetSize.height() > 0)
    {
        raw = raw.scaled(targetSize,
                         Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
    }

    pix = raw;
    QPixmapCache::insert(cacheKey, pix);

    return true;
}

QVector<QPixmap> loadClip(const QString &roleName,
                          Direction dir,
                          Action act,
                          const QSize &targetSize)
{
    static QHash<QString, QVector<QPixmap>> clipCache;

    QString subFolder = subFolderByRole(roleName);

    QString baseName = QString("%1_%2_%3")
                           .arg(roleName)
                           .arg(Character::Direcname(dir))
                           .arg(Character::Actionname(act));

    QString cacheKey = subFolder + "/" + baseName + QString("@%1x%2")
                                                        .arg(targetSize.width())
                                                        .arg(targetSize.height());

    if (clipCache.contains(cacheKey))
    {
        return clipCache.value(cacheKey);
    }

    QVector<QPixmap> clip;


    for (int i = 0; i <= 30; i++)
    {
        QString fileName = QString("%1_%2.png").arg(baseName).arg(i);

        QPixmap frame;

        if (loadOnePixmap(frame, subFolder, fileName, targetSize))
        {
            clip.push_back(frame);
        }
    }

    if (clip.isEmpty())
    {
        QString singleFileName = baseName + ".png";

        QPixmap single;

        if (loadOnePixmap(single, subFolder, singleFileName, targetSize))
        {
            clip.push_back(single);
        }
    }

    if (clip.isEmpty())
    {
        QString roleSingleFileName = roleName + ".png";

        QPixmap roleSingle;

        if (loadOnePixmap(roleSingle, subFolder, roleSingleFileName, targetSize))
        {
            clip.push_back(roleSingle);
        }
    }

    clipCache.insert(cacheKey, clip);
    return clip;
}


Character::Character()
    : Entity(),
    hp(10),
    maxhp(10),
    speed(3),
    attack(1),
    direction(Direction::right),
    action(Action::still)
{
}

Character::Character(double x, double y, double w, double h)
    : Entity(x, y, w, h),
    hp(10),
    maxhp(10),
    speed(3),
    attack(1),
    direction(Direction::right),
    action(Action::still)
{
}

void Character::load()
{
}

void Character::hurt(double att)
{
    if (!ealive) return;

    hp -= att;

    if (hp <= 0)
    {
        hp = 0;
        ealive = false;
    }
}

Direction Character::getDirection()
{
    return direction;
}

Action Character::getAction()
{
    return action;
}

void Character::setDirection(Direction d)
{
    direction = d;
}

void Character::setAction(Action a)
{
    action = a;
}

QString Character::Actionname(Action a)
{
    switch (a)
    {
    case Action::still:
        return "still";
    case Action::run:
        return "run";
    case Action::attack:
        return "attack";
    case Action::ha:
        return "ha";
    case Action::plusattack:
        return "plusattack";
    }

    return "still";
}

QString Character::Direcname(Direction d)
{
    switch (d)
    {
    case Direction::up:
        return "up";
    case Direction::down:
        return "down";
    case Direction::left:
        return "left";
    case Direction::right:
        return "right";
    }

    return "right";
}

void Character::loadaction(const QString &roleName)
{
    role = roleName;

    QSize targetSize = esize.toSize();

    for (int a = 0; a < ACTION; a++)
    {
        for (int d = 0; d < DIRECT; d++)
        {
            Action act = static_cast<Action>(a);
            Direction dir = static_cast<Direction>(d);

            sprites[a][d] = loadClip(roleName, dir, act, targetSize);
        }
    }

    int still = static_cast<int>(Action::still);
    int right = static_cast<int>(Direction::right);

    if (!sprites[still][right].isEmpty())
    {
        epixmap = sprites[still][right][0];
    }
}

void Character::changeRole(const QString &roleName)
{
    loadaction(roleName);

    frameid = 0;
    frametick = 0;
    action = Action::still;
    direction = Direction::right;
}

QPixmap Character::currentPixmap()
{
    int a = static_cast<int>(action);
    int d = static_cast<int>(direction);

    int still = static_cast<int>(Action::still);
    int right = static_cast<int>(Direction::right);
    int left = static_cast<int>(Direction::left);
    int down = static_cast<int>(Direction::down);
    int up = static_cast<int>(Direction::up);

    if (a >= 0 && a < ACTION && d >= 0 && d < DIRECT)
    {
        // 1. 优先使用当前动作 + 当前方向
        if (!sprites[a][d].isEmpty())
        {
            int id = frameid % sprites[a][d].size();
            return sprites[a][d][id];
        }

        if (!sprites[still][d].isEmpty())
        {
            int id = frameid % sprites[still][d].size();
            return sprites[still][d][id];
        }
    }

    return epixmap;
}
void Character::startAction(Action a, int keepTicks)
{
    action = a;
    actionLock = keepTicks;

    frameid = 0;
    frametick = 0;
    lastAction = a;
    lastDirection = direction;
}

void Character::playaction(Action a, int lockTicks)
{
    action = a;
    actionLock = lockTicks;

    frameid = 0;
    frametick = 0;
    lastAction = a;
    lastDirection = direction;
}

bool Character::actionLocked()
{
    return actionLock > 0;
}

bool Character::isActionLocked()
{
    return actionLock > 0;
}

void Character::updateanimation()
{
    int a = static_cast<int>(action);
    int d = static_cast<int>(direction);

    if (action != lastAction || direction != lastDirection)
    {
        frameid = 0;
        frametick = 0;
        lastAction = action;
        lastDirection = direction;
    }

    if (a < 0 || a >= ACTION || d < 0 || d >= DIRECT)
    {
        return;
    }

    QVector<QPixmap> *clip = nullptr;

    if (!sprites[a][d].isEmpty())
    {
        clip = &sprites[a][d];
    }
    else
    {
        int still = static_cast<int>(Action::still);

        if (!sprites[still][d].isEmpty())
        {
            clip = &sprites[still][d];
        }
    }

    if (clip == nullptr || clip->size() <= 1)
    {
        return;
    }

    frametick++;

    if (frametick >= frameinterval)
    {
        frametick = 0;
        frameid++;

        if (frameid >= clip->size())
        {
            frameid = 0;
        }
    }
}

void Character::updateaction()
{
    updateanimation();
    updateactionlock();
}

void Character::updateactionlock()
{
    if (actionLock > 0)
    {
        actionLock--;
    }
}

void Character::updategame(double mapw, double maph)
{
    Q_UNUSED(mapw);
    Q_UNUSED(maph);
}

void Character::addStiff(int ticks)
{
    if (ticks > stiffTicks)
    {
        stiffTicks = ticks;
    }
}

void Character::updateStiff()
{
    if (stiffTicks > 0)
    {
        stiffTicks--;
    }
}

bool Character::isStiff()
{
    return stiffTicks > 0;
}