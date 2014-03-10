#include "filereader.h"

namespace iSimParse {
const QString REGEXP_ID = "\"([^\"]*)\"";
const QString REGEXP_SEP = " *,? *";
const QString REGEXP_NUM = "((?:0x[0-9a-fA-F]+)|(?:[0-9]+))";
const QRegExp REGEXP_LINE = QRegExp(QString("\\(") + REGEXP_ID + REGEXP_SEP + REGEXP_NUM + REGEXP_SEP + REGEXP_NUM + REGEXP_SEP + "\\{([^{]*)\\}" + " *\\)");
const QString REGEXP_PROP_SEP = " *: *";
const QRegExp REGEXP_PROP =  QRegExp(QString(" *") + REGEXP_ID + REGEXP_PROP_SEP  + REGEXP_ID + REGEXP_SEP);
const QChar PARSE_BRACKET_OPEN_OLD_FORMAT = '(';
const QChar PARSE_BRACKET_CLOSE_OLD_FORMAT = ')';
const QChar PARSE_BRACKET_OPEN_NEW_FORMAT = '{';
const QChar PARSE_BRACKET_CLOSE_NEW_FORMAT = '}';
const QString PARSE_KEYWORD_INCIDENT = "Incident";
const QString PARSE_KEYWORD_STATE_SEGMENT = "segmentState";
const QString PARSE_KEYWORD_TRAFFIC_SINGAL = "TrafficSignal";
const QString PARSE_KEYWORD_TRAFFIC_SINGAL_UPDATE = "TrafficSignalUpdate";
const QString PARSE_KEYWORD_DONE_NETWORK = "ROADNETWORK_DONE";
const QString PARSE_KEYWORD_STATE_DRIVER = "Driver";
const QString PARSE_KEYWORD_STATE_BUSDRIVER = "BusDriver";
const QString PARSE_KEYWORD_STATE_PEDESTRIAN = "pedestrian";
const QString PARSE_KEYWORD_STATE_PASSENGER = "passenger";
const QString PARSE_KEYWORD_TYPE_UNINODE = "uni-node";
const QString PARSE_KEYWORD_TYPE_MULTINODE = "multi-node";
const QString PARSE_KEYWORD_TYPE_LINK = "link";
const QString PARSE_KEYWORD_TYPE_ROADSEGMENT = "road-segment";
const QString PARSE_KEYWORD_TYPE_POLYLINE = "polyline";
const QString PARSE_KEYWORD_TYPE_LANE = "lane";
const QString PARSE_KEYWORD_TYPE_LANECONNECTOR = "lane-connector";
const QString PARSE_KEYWORD_TYPE_GRAPH = "sd-graph";
const QString PARSE_KEYWORD_TYPE_VERTEX = "sd-vertex";
const QString PARSE_KEYWORD_TYPE_EDGE = "sd-edge";
const QString PARSE_KEYWORD_TYPE_BUSSTOP = "busstop";
const QString PARSE_KEYWORD_TYPE_CROSSING = "crossing";
const QString PARSE_KEYWORD_PROP_TIMESLICE = "time-slice";
const QString PARSE_KEYWORD_PROP_SPEED = "speed";
const QString PARSE_KEYWORD_PROP_FLOW = "flow";
const QString PARSE_KEYWORD_PROP_DENSITY = "density";
const QString PARSE_KEYWORD_PROP_LOCATIONX = "xPos";
const QString PARSE_KEYWORD_PROP_LOCATIONY = "yPos";
const QString PARSE_KEYWORD_PROP_ANGLE = "angle";
const QString PARSE_KEYWORD_PROP_LENGTH = "length";
const QString PARSE_KEYWORD_PROP_PASSENGERS = "passengers";
const QString PARSE_KEYWORD_PROP_ARRIVALTIME = "real_ArrivalTime";
const QString PARSE_KEYWORD_PROP_DWELLTIME = "DwellTime_ijk";
const QString PARSE_KEYWORD_PROP_BUSLINEID = "buslineID";
const QString PARSE_KEYWORD_PROP_ROADNAME = "road-name";
const QString PARSE_KEYWORD_PROP_STARTNODE = "start-node";
const QString PARSE_KEYWORD_PROP_ENDNODE = "end-node";
const QString PARSE_KEYWORD_PROP_FWDPATH = "fwd-path";
const QString PARSE_KEYWORD_PROP_REVPATH = "rev-path";
const QString PARSE_KEYWORD_PROP_PARENTLINK = "parent-link";
const QString PARSE_KEYWORD_PROP_MAXSPEED = "max-speed";
const QString PARSE_KEYWORD_PROP_WIDTH = "width";
const QString PARSE_KEYWORD_PROP_MANDATORY = "mandatory";
const QString PARSE_KEYWORD_PROP_INFO = "info";
const QString PARSE_KEYWORD_PROP_CURRSEGMENT = "curr-segment";
const QString PARSE_KEYWORD_PROP_FWDSPEED = "fwd-speed";
const QString PARSE_KEYWORD_PROP_FWDACCEL = "fwd-accel";
const QString PARSE_KEYWORD_PROP_LANES = "lanes";
const QString PARSE_KEYWORD_PROP_FROMNODE = "from-node";
const QString PARSE_KEYWORD_PROP_TONODE = "to-node";
const QString PARSE_KEYWORD_PROP_AIMSUNID = "aimsun-id";
const QString PARSE_KEYWORD_PROP_HEXID = "hex_id";
const QString PARSE_KEYWORD_PROP_ID = "id";
const QString PARSE_KEYWORD_SEGMENT_AIMSUNID = "segment_aimsun_id";
const QString PARSE_KEYWORD_VISIBILITY = "visibility";
const QString PARSE_KEYWORD_POSITION = "position";
const QString PARSE_KEYWORD_SEVERITY = "severity";
const QString PARSE_KEYWORD_LENGTH = "length";
const QString PARSE_KEYWORD_CAP_FACTOR = "cap_factor";
const QString PARSE_KEYWORD_START_TIME = "start_time";
const QString PARSE_KEYWORD_DURATION = "duration";
const QString PARSE_KEYWORD_SPEED_LIMIT = "speed_limit";
const QString PARSE_KEYWORD_X_LANE_STARTPOINT = "xLaneStartPos";
const QString PARSE_KEYWORD_Y_LANE_STARTPOINT = "yLaneStartPos";
const QString PARSE_KEYWORD_X_LANE_ENDPOINT = "xLaneEndPos";
const QString PARSE_KEYWORD_Y_LANE_ENDPOINT = "yLaneEndPos";
const QString PARSE_KEYWORD_PROP_NODE= "node";
const QString PARSE_KEYWORD_PROP_FRAME= "frame";
const QString PARSE_KEYWORD_PROP_PHASE= "phases";
const QString PARSE_KEYWORD_PROP_CURRPHASE = "currPhase";
const QString PARSE_KEYWORD_PROP_PARENTSEGMENT = "parent-segment";
const QString PARSE_KEYWORD_PROP_POINTS = "points";
const QString PARSE_KEYWORD_PROP_LANE_ARG = "lane-%1";
const QString PARSE_KEYWORD_PROP_LINEISSIDEWALK_ARG = "line-%1is-sidewalk";
const QString LANECONNECTOR_FROMSEGMENT = "from-segment";
const QString LANECONNECTOR_TOSEGMENT = "to-segment";
const QString LANECONNECTOR_FROMLANEINDEX = "from-lane";
const QString LANECONNECTOR_TOLANEINDEX = "to-lane";
}

FileReader::FileReader(GeospatialIndex *geospatialIndex, TemporalIndex *temporalIndex):
    geospatialIndex_(geospatialIndex), temporalIndex_(temporalIndex), fileHandle_(0) {
    needStop_ = false;
}

FileReader::~FileReader() {
    if (fileHandle_) {
        fileHandle_->close();
        delete fileHandle_;
    }
}

void FileReader::stopReader() {
    needStop_ = true;
}

void FileReader::setTarget(QString path) {
    path_ = path;
    isSpatialDataFinished_ = false;
    isTemporalDataDetected_ = false;
    if (fileHandle_) {
        fileHandle_->close();
        delete fileHandle_;
    }
    fileHandle_ = new QFile(path_);
}

QPointF FileReader::genCoordinate(double x, double y) {
    return QPointF(x, -y);
}

void FileReader::run() {

    emit announceLog(tr("Start loading the file %1").arg(path_));
    if (fileHandle_ == 0) {
        emit announceError(tr("there is no selected file."));
        return;
    }
    if (!fileHandle_->open(QIODevice::ReadOnly|QIODevice::Text)) {
        emit announceError(tr("unable to open the file."));
        return;
    }
    long fileSize = fileHandle_->size();
    if (fileSize == 0) {
        emit announceError(tr("the file is empty."));
        return;
    }

    emit announceStatus(tr("Loading input file .."));
    needStop_ = false;
    QTextStream readStream(fileHandle_);
    long countLine = 0;
    while (!readStream.atEnd() && !needStop_) {
        QString line = readStream.readLine().trimmed();
        if (!line.isEmpty()) { // Ignore empty lines
            if (line.contains(iSimParse::PARSE_KEYWORD_DONE_NETWORK, Qt::CaseSensitive)) {
                isSpatialDataFinished_ = true;
                emit announceSpatialDataFinished();
            }
            if (!processLine(line)) {
                QString log = QString("Skipping the current line at byte position %1 / %2.").arg(readStream.pos()).arg(fileHandle_->size());
                emit announceLog(log);
            }
        }
        countLine++;
        if (countLine % 100 == 0)
           emit announceProgressUpdated(fileHandle_->pos()*100/fileSize);
    }

    fileHandle_->close();
    if (needStop_) {
        emit announceLog("The file loading is aborted from user.");
    } else {
        emit announceCompleted();
    }
}

bool FileReader::processLine(QString &line) {
    if (line.length() < 3) return false;
    QChar firstChar = *(line.begin());
    QChar lastChar = *(line.end()-1);

    // Determine whether line is old style or new JSON style
    if (firstChar == iSimParse::PARSE_BRACKET_OPEN_OLD_FORMAT && lastChar == iSimParse::PARSE_BRACKET_CLOSE_OLD_FORMAT) {
        // Old-style line, of the form:
        // ("obj-type", frame_ID, obj_ID, {"key1":"val1","key2":"val2"...,})
        if (iSimParse::REGEXP_LINE.indexIn(line) == -1 ) {
            emit announceLog(tr("invalid format line: %1").arg(line));
            return false;
        }

        //Retrieve mandatory fields: object type, frame ID, and object ID
        QString objType = iSimParse::REGEXP_LINE.cap(1);

        //check what type of data it is
        bool isSegmentState = (objType == iSimParse::PARSE_KEYWORD_STATE_SEGMENT);
        bool isAgent = (objType == iSimParse::PARSE_KEYWORD_STATE_DRIVER ||
                        objType == iSimParse::PARSE_KEYWORD_STATE_BUSDRIVER ||
                        objType == iSimParse::PARSE_KEYWORD_STATE_PEDESTRIAN ||
                        objType == iSimParse::PARSE_KEYWORD_STATE_PASSENGER);
        bool isTemporalData = (isSegmentState || isAgent);

        unsigned int frameID = iSimParse::REGEXP_LINE.cap(2).toUInt();
        unsigned long objID ;
        if(objType=="sd-vertex" || objType =="sd-edge") {
            objID = iSimParse::REGEXP_LINE.cap(3).toInt();
        } else if (isAgent) {
            objID = iSimParse::REGEXP_LINE.cap(3).toLong();
        } else {
            objID = iSimParse::REGEXP_LINE.cap(3).toLong(0, 16);
        }

        //Build up a map of properties based on the key, value pairs enclosed with {}.
        QMap<QString, QString> properties;
        int matchID = 0;
        QString propSrc = iSimParse::REGEXP_LINE.cap(4);
        while (true) {
            matchID = iSimParse::REGEXP_PROP.indexIn(propSrc, matchID);
            if (matchID<0) {
                break;
            }
            properties[iSimParse::REGEXP_PROP.cap(1)] = iSimParse::REGEXP_PROP.cap(2);
            matchID += iSimParse::REGEXP_PROP.cap(0).length();
        }

        // determine which data object to create
        if (isTemporalData) {
            // If it's the first dynamic data line, we have to tell the main thread so that it can continue
            // processing the spatial data
            if (!isSpatialDataFinished_) {
                isSpatialDataFinished_ = true;
                emit announceSpatialDataFinished();
            }

            if (!isTemporalDataDetected_) {
                isTemporalDataDetected_ = true;
                emit announceTemporalDataExists();
            }

            if(isSegmentState) { // parse mesoscopic data
                createMesoscopic(objID, frameID, properties);
            } else {
                createAgent(objType, objID, frameID, properties);
            }
        } else { // parse road network data -- INCLUDES BUSSTOP
            parseRoadNetworkDataLine(objType.toLower(), objID, properties);
        }
    } else if (firstChar==iSimParse::PARSE_BRACKET_OPEN_NEW_FORMAT && lastChar==iSimParse::PARSE_BRACKET_CLOSE_NEW_FORMAT) {
        // New-style line, of the form:
        // JSon
        //currently only used for traffic signals & incident
        QVariantMap result = QtJson::parse(line).toMap();
        if (result.contains(iSimParse::PARSE_KEYWORD_TRAFFIC_SINGAL)) {
            createTrafficSignal(result[iSimParse::PARSE_KEYWORD_TRAFFIC_SINGAL].toMap());
        } else if (result.contains(iSimParse::PARSE_KEYWORD_TRAFFIC_SINGAL_UPDATE)) {
            createPhaseData(result[iSimParse::PARSE_KEYWORD_TRAFFIC_SINGAL_UPDATE].toMap());
        }
    } // Anything else is assumed to be a comment
    return true;
}

bool FileReader::createMesoscopic(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    // ("segmentState",0,0x37dc2d0,{"speed":"1388.8889","flow":"0","density":"0"})

    Mesoscopic* mesoscopic = new Mesoscopic();
    mesoscopic->setSegmentID(objID);
    mesoscopic->setTick(frameID);

    // Get speed
    QMap<QString, QString>::iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_SPEED);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Mesoscopic %1 does not have speed value").arg(objID));
        return false;
    }
    mesoscopic->setSpeed(QString(propertiesIter.value()).toDouble());

    // Get flow
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_FLOW);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Mesoscopic %1 does not have flow value").arg(objID));
        return false;
    }
    mesoscopic->setFlow(QString(propertiesIter.value()).toDouble());

    // Get density
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_DENSITY);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Mesoscopic %1 data does not have density value").arg(objID));
        return false;
    }
    mesoscopic->setDensity(QString(propertiesIter.value()).toDouble());

    temporalIndex_->insert(mesoscopic);
    return true;
}

bool FileReader::createAgent(const QString& objType, unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    if (objType == iSimParse::PARSE_KEYWORD_STATE_DRIVER)  return createDriver(objID, frameID, properties);
    if (objType == iSimParse::PARSE_KEYWORD_STATE_BUSDRIVER) return createBusDriver(objID, frameID, properties);
    if (objType == iSimParse::PARSE_KEYWORD_STATE_PEDESTRIAN || objType == iSimParse::PARSE_KEYWORD_STATE_PASSENGER) {
        return createPedestrian(objID, frameID, properties);
    }
    return false;
}

bool FileReader::createDriver(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    //("Driver",11,47,{"xPos":"37271822","yPos":"14399409","angle":"128.89281","length":"400","width":"200"})
    // optional {"curr-segment":"0xa381ae0", "fwd-speed":"50","fwd-accel":"500", "info":"MLC","mandatory":"0"}
    double xPos = 0, yPos = 0;
    double angle, length, width;
    unsigned long currentSegment = 0;
    int mandatory = -1, fwdSpeed = 0, fwdAccel = 0;
    QString info = "";

    QMap<QString, QString>::iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONX);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Driver %1 does not have xPos value").arg(objID));
        return false;
    }
    xPos = QString(propertiesIter.value()).toDouble();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONY);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Driver %1 does not have yPos value").arg(objID));
        return false;
    }
    yPos = QString(propertiesIter.value()).toDouble();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_ANGLE);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Driver %1 does not have angle value").arg(objID));
        return false;
    }
    angle = QString(propertiesIter.value()).toDouble();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LENGTH);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Driver %1 does not have length value").arg(objID));
        return false;
    }
    length = QString(propertiesIter.value()).toDouble();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_WIDTH);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Driver %1 does not have width value").arg(objID));
        return false;
    }
    width = QString(propertiesIter.value()).toDouble();

    // get curr-segment (optional)
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_CURRSEGMENT);
    if (propertiesIter!=properties.end()) {
        currentSegment = QString(propertiesIter.value()).toULong(0, 16);
    }

    // get fwd-speed (optional)
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_FWDSPEED);
    if (propertiesIter!=properties.end()) {
        fwdSpeed = QString(propertiesIter.value()).toInt();
    }

    // get fwd-accel (optional)
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_FWDACCEL);
    if (propertiesIter!=properties.end()) {
        fwdAccel = QString(propertiesIter.value()).toInt();
    }

    // Get info (optional)
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_INFO);
    if (propertiesIter!=properties.end()) {
        info = QString(propertiesIter.value());
    }

    // Get mandatory (optional)
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_MANDATORY);
    if (propertiesIter!=properties.end()) {
        mandatory = QString(propertiesIter.value()).toInt();
    }

    Driver driver(objID, frameID, genCoordinate(xPos, yPos), angle, length, width, currentSegment, fwdSpeed, fwdAccel, mandatory, info);
    temporalIndex_->insert(driver);

    return true;
}

bool FileReader::createBusDriver(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    //("BusDriver",2449,2496,{"xPos":"37267169","yPos":"14352701","angle":"127.34147","length":"3600","width":"400","passengers":"0","real_ArrivalTime":"243400","DwellTime_ijk":"0","buslineID":"30_2"})

    double xPos, yPos, angle, length, width;
    long realArrivalTime;
    int passengers = 0, dwellTime;
    QString busLineID = "";

    // Get road segment
    QMap<QString, QString>::iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONX);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("BusDriver %1 does not have xPos value").arg(objID));
        return false;
    }
    xPos = QString(propertiesIter.value()).toDouble();

    // Get lane
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONY);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("BusDriver %1 does not have yPos value").arg(objID));
        return false;
    }
    yPos = QString(propertiesIter.value()).toDouble();

    // Get angle
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_ANGLE);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("BusDriver %1 does not have angle value").arg(objID));
        return false;
    }
    angle = QString(propertiesIter.value()).toDouble();

    // Get length
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LENGTH);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("BusDriver %1 does not have length value").arg(objID));
        return false;
    }
    length = QString(propertiesIter.value()).toDouble();

    // Get width
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_WIDTH);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("BusDriver %1 does not have width value").arg(objID));
        return false;
    }
    width = QString(propertiesIter.value()).toDouble();

    // Get other bus related properties
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_PASSENGERS);
    passengers = QString(propertiesIter.value()).toInt();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_ARRIVALTIME);
    realArrivalTime = QString(propertiesIter.value()).toLong();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_DWELLTIME);
    dwellTime = QString(propertiesIter.value()).toInt();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_BUSLINEID);
    busLineID = QString(propertiesIter.value());

    // Create microscopic object
    BusDriver bus(objID, frameID, genCoordinate(xPos, yPos), angle, length, width, passengers, realArrivalTime, dwellTime, busLineID);

    temporalIndex_->insert(bus);
    return true;
}

bool FileReader::createPedestrian(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    //("pedestrian",2449,3792,{"xPos":"37285386","yPos":"14393768",})
    double xPos = 0, yPos = 0;

    // Get road segment
    QMap<QString, QString>::iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONX);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Pedestrian %1 does not have xPos value").arg(objID));
        return false;
    }
    xPos = QString(propertiesIter.value()).toDouble();

    // Get lane
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONY);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Pedestrian %1 does not have yPos value").arg(objID));
        return false;
    }
    yPos = QString(propertiesIter.value()).toDouble();

    // Create microscopic object
    Pedestrian pedestrian(objID, frameID, genCoordinate(xPos, yPos));

    temporalIndex_->insert(pedestrian);
    return true;
}

bool FileReader::parseRoadNetworkDataLine(const QString& objType, unsigned long objID, QMap<QString, QString> &properties) {
    // Handle uni nodes and multi nodes similarly

    // Enqueue to uninode raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_MULTINODE) return createMultiNode(objID, properties);
    // Enqueue to uninode raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_UNINODE) return createUniNode(objID, properties);
    // Enqueue to link raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_LINK) return createLink(objID, properties);
    // Enqueue to road segments raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_ROADSEGMENT ) return createRoadSegment(objID, properties);
    // Enqueue to polylines raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_POLYLINE ) return createPolyline(objID, properties);
    // Enqueue to lanes raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_LANE ) return createLane(properties);

    if (objType==iSimParse::PARSE_KEYWORD_TYPE_LANECONNECTOR ) return createLaneConnector(objID, properties);
    /*
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_GRAPH ) return createGraph(objID, properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_VERTEX ) return createVertex(objID, properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_EDGE ) return createEdge(objID, properties);
    */
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_BUSSTOP) return createBusstop(objID, properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_CROSSING) return createCrossing(objID, properties);

    return false;
}

bool FileReader::createMultiNode(unsigned long id, QMap<QString, QString> &properties) {
    double xPos = 0, yPos = 0;
    unsigned long aimsunId = 0;

    // Get x position
    QMap<QString, QString>::const_iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONX);
    xPos = (propertiesIter==properties.end()) ? -1 : propertiesIter.value().toDouble();
    // Get y position
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONY);
    yPos = (propertiesIter==properties.end()) ? -1 : propertiesIter.value().toDouble();
    // Check if properties are valid
    if (xPos == -1 || yPos == -1) {
        emit announceLog(tr("MultiNode %1 does not have valid property xPos or yPos.").arg(id));
        return false;
    }
    // get aimsunId
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_AIMSUNID);
    if (propertiesIter!=properties.end()) {
        aimsunId = propertiesIter.value().toULong(0, 10);
    }
    // Create data object
    MultiNode *multiNode = new MultiNode(id, genCoordinate(xPos, yPos), aimsunId);
    geospatialIndex_->insert(multiNode);
    return true;
}

bool FileReader::createUniNode(unsigned long id, QMap<QString, QString> &properties) {
    double xPos = 0, yPos = 0;
    unsigned long aimsunId = 0;

    // Get x position
    QMap<QString, QString>::const_iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONX);
    xPos = (propertiesIter==properties.end()) ? -1 : propertiesIter.value().toDouble();
    // Get y position
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LOCATIONY);
    yPos = (propertiesIter==properties.end()) ? -1 : propertiesIter.value().toDouble();
    // Check if properties are valid
    if (xPos == -1 || yPos == -1) {
        emit announceLog(tr("UniNode %1 does not have property xPos or yPos.").arg(id));
        return false;
    }
    // get aimsunId
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_AIMSUNID);
    if (propertiesIter != properties.end()) {
        aimsunId = propertiesIter.value().toULong(0, 10);
    }
    // Create data object
    UniNode *uniNode = new UniNode(id, genCoordinate(xPos, yPos), aimsunId);
    geospatialIndex_->insert(uniNode);
    return true;
}

bool FileReader::createLink(unsigned long id, QMap<QString, QString> &properties) {
    unsigned long startNodeId = 0, endNodeId = 0;
    QString name;

    QMap<QString, QString>::const_iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_ROADNAME);
    name = (propertiesIter==properties.end()) ? "" : propertiesIter.value();

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_STARTNODE);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Link %1 does not have a valid start node id").arg(id));
        return false;
    }
    startNodeId = propertiesIter.value().toULong(0, 16);

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_ENDNODE);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Link %1 does not have a valid end nodeid").arg(id));
        return false;
    }
    endNodeId = propertiesIter.value().toULong(0, 16);

    Link *link = new Link(id, name, startNodeId, endNodeId);

    // Get fwd-path (list of segment IDs)
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_FWDPATH);
    if (propertiesIter!=properties.end()) {
        QString fwdPath = propertiesIter.value();
        fwdPath.replace(QRegExp("[\\[ \\]]"), "");
        QStringList segmentID_List = fwdPath.split(",");
        for (int i=0; i<segmentID_List.size(); i++) {
            if (!segmentID_List[i].isEmpty()) {
                link->addFwdSegmentId(segmentID_List[i].toULong(0, 16));
            }
        }
    }

    geospatialIndex_->insert(link);
    return true;
}

bool FileReader::createRoadSegment(unsigned long id, QMap<QString, QString> &properties) {
    unsigned long parentLinkId = 0, fromNodeId = 0, toNodeId = 0, aimsunId = 0;
    int maxSpeed = -1, width = -1, nLane = -1;

    QMap<QString, QString>::const_iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_PARENTLINK);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Road segment %1 does not have a valid parent link ID").arg(id));
        return false;
    }
    parentLinkId = propertiesIter.value().toULong(0, 16);

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_MAXSPEED);
    if (propertiesIter != properties.end()) {
        maxSpeed = propertiesIter.value().toInt();
    }

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_WIDTH);
    if (propertiesIter != properties.end()) {
        width = propertiesIter.value().toInt();
    }

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_LANES);
    if (propertiesIter != properties.end()) {
        nLane = propertiesIter.value().toInt();
    }

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_FROMNODE);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Road segment %1 does not have a valid from node id").arg(id));
        return false;
    }
    fromNodeId = propertiesIter.value().toULong(0, 16);

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_TONODE);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Road segment %1 does not have a valid to node id").arg(id));
        return false;
    }
    toNodeId = propertiesIter.value().toULong(0, 16);

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_AIMSUNID);
    if (propertiesIter != properties.end()) {
        aimsunId = propertiesIter.value().toULong(0, 10);
    }

    RoadSegment *roadSegment = new RoadSegment(parentLinkId, id, aimsunId, fromNodeId, toNodeId, maxSpeed, width, nLane);
    geospatialIndex_->insert(roadSegment);
    return true;
}

bool FileReader::createPolyline(unsigned long id, QMap<QString, QString> &properties)  {

    QMap<QString, QString>::const_iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_PARENTSEGMENT);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Segment Polyline %1 does not have a valid parent segment id.").arg(id));
        return false;
    }
    unsigned long parentSegmentID = propertiesIter.value().toULong(0, 16);

    RoadSegment *segment = geospatialIndex_->getRoadSegemnt(parentSegmentID);
    if (segment == 0) {
        emit announceLog(tr("Segment Polyline %1 - unable to find segment parent %2.").arg(id).arg(parentSegmentID));
        return false;
    }

    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_POINTS);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Polyline %1 doesn not have a valid set of points.").arg(id));
        return false;
    }
    QString setPointsStr = propertiesIter.value();
    setPointsStr.remove(QRegExp("[\\[ \\]]"));
    QRegExp roundBrackets("\\(\\d+\\,\\d+\\)");
    QRegExp roundBracketsSeries("(\\d+)(\\,)(\\d+)");

    int matchID = 0;
    while (true) {
        matchID = roundBrackets.indexIn(setPointsStr, matchID);
        if (matchID<0) {
            break;
        }

        QString pointStr = roundBrackets.cap(0);
        if (roundBracketsSeries.indexIn(pointStr)) {
            segment->addPointToPolyline(genCoordinate(roundBracketsSeries.cap(1).toDouble(), roundBracketsSeries.cap(3).toDouble()));
        }
        matchID += roundBrackets.cap(0).length();
    }

    return true;
}

bool FileReader::createLane(QMap<QString, QString> &properties) {

    QMap<QString, QString>::const_iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_PARENTSEGMENT);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Lane does not have a valid segment parent id"));
        return false;
    }
    unsigned long parentSegmentID = propertiesIter.value().toULong(0, 16);
    RoadSegment *segment = geospatialIndex_->getRoadSegemnt(parentSegmentID);
    if (segment == 0) {
        emit announceLog(tr("Unable to find segment %1").arg(parentSegmentID));
        return false;
    }

    unsigned int index = 0;
    QRegExp squareBrackets("[\\[ \\]]");
    QRegExp roundBrackets("\\(\\d+\\,\\d+\\)");
    QRegExp roundBracketsSeries("(\\d+)(\\,)(\\d+)");
    while (true) {
        //check side walk
        bool isSideWalk = false;
        QMap<QString, QString>::const_iterator hasSideWalkKeyword = properties.find(QString(iSimParse::PARSE_KEYWORD_PROP_LINEISSIDEWALK_ARG.arg(index)));
        if (hasSideWalkKeyword != properties.end()) {
            isSideWalk = true;
        }
        Lane *lane = new Lane(parentSegmentID, index, isSideWalk);

        propertiesIter = properties.find(QString(iSimParse::PARSE_KEYWORD_PROP_LANE_ARG).arg(index));
        if (propertiesIter == properties.end()) {
            break;
        }
        // add points
        QString setPointsStr = propertiesIter.value();
        setPointsStr.remove(squareBrackets);
        int matchID = 0;
        while (true) {
            matchID = roundBrackets.indexIn(setPointsStr, matchID);
            if (matchID<0) {
                break;
            }

            QString pointStr = roundBrackets.cap(0);
            if (roundBracketsSeries.indexIn(pointStr)) {
                lane->addPointToPolyline(genCoordinate(roundBracketsSeries.cap(1).toDouble(),
                                                       roundBracketsSeries.cap(3).toDouble()));
            }
            matchID += roundBrackets.cap(0).length();
        }
        segment->addLane(lane);
        index++;
    }

    return true;
}

bool FileReader::createLaneConnector(unsigned long id, QMap<QString, QString> &properties) {
    unsigned long fromSegment = 0, toSegment = 0;
    unsigned int fromLane = 0, toLane = 0;
    QMap<QString, QString>::const_iterator propertiesIter = properties.find(iSimParse::LANECONNECTOR_FROMSEGMENT);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Lane Connector %1 does not have a valid from segment id.").arg(id));
        return false;
    }
    fromSegment = propertiesIter.value().toULong(0, 16);

    propertiesIter = properties.find(iSimParse::LANECONNECTOR_FROMLANEINDEX);
    if (propertiesIter==properties.end()) {
         emit announceLog(tr("Lane Connector %1 does not have a valid from lane index.").arg(id));
        return false;
    }
    fromLane = propertiesIter.value().toInt();

    propertiesIter = properties.find(iSimParse::LANECONNECTOR_TOSEGMENT);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Lane Connector %1 does not have a valid to segment id.").arg(id));
        return false;
    }
    toSegment = propertiesIter.value().toULong(0, 16);

    propertiesIter = properties.find(iSimParse::LANECONNECTOR_TOLANEINDEX);
    if (propertiesIter==properties.end()) {
         emit announceLog(tr("Lane Connector %1 does not have a valid to lane index.").arg(id));
        return false;
    }
    toLane = propertiesIter.value().toInt();

    LaneConnector* laneConnector = new LaneConnector(id, fromSegment, fromLane, toSegment, toLane);
    geospatialIndex_->insert(laneConnector);
    return true;
}

bool FileReader::createBusstop(unsigned long id, QMap<QString, QString> &properties) {
    QMap<QString, QString>::const_iterator propertiesIter = properties.find("near-1");
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Bus stop %1 does not have near-1 pos").arg(id));
        return false;
    }
    QString near_1 = propertiesIter.value();
    QStringList near1Split = near_1.split(",");

    // get aimsunId
    unsigned long aimsunId = 0;
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_AIMSUNID);
    if (propertiesIter!=properties.end()) {
        aimsunId = propertiesIter.value().toULong(0, 10);
    }

    BusStop *busStop = new BusStop(id, aimsunId, genCoordinate(near1Split.at(0).toDouble(), near1Split.at(1).toDouble()));
    geospatialIndex_->insert(busStop);
    return true;
}

bool FileReader::createCrossing(unsigned long id, QMap<QString, QString> &properties) {
    // ("crossing", 0, 0x45097f0, {"near-1":"37263607,14387486","near-2":"37262529,14388590","far-1":"37263790,14387681","far-2":"37262743,14388779",})

    //get near-1
    QMap<QString, QString>::const_iterator propertiesIter = properties.find("near-1");
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("crossing %1 does not have near-1 pos").arg(id));
        return false;
    }
    QString near_1 = propertiesIter.value();
    QStringList near1Split = near_1.split(",");
    QPointF p1 = genCoordinate(near1Split.at(0).toDouble(), near1Split.at(1).toDouble());

    //get near-2
    propertiesIter = properties.find("near-2");
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("crossing %1 does not have near-2 pos").arg(id));
        return false;
    }
    QString near_2 = propertiesIter.value();
    QStringList near2Split = near_2.split(",");
    QPointF p2 = genCoordinate(near2Split.at(0).toDouble(), near2Split.at(1).toDouble());

    //get far-1
    propertiesIter = properties.find("far-1");
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("crossing %1 does not have far-1 pos").arg(id));
        return false;
    }
    QString far_1= propertiesIter.value();
    QStringList far1Split = far_1.split(",");
    QPointF p4 = genCoordinate(far1Split.at(0).toDouble(), far1Split.at(1).toDouble());

    //get far-2
    propertiesIter = properties.find("far-2");
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("crossing %1 does not have far-2 pos").arg(id));
        return false;
    }
    QString far_2 = propertiesIter.value();
    QStringList far2Split = far_2.split(",");
    QPointF p3 = genCoordinate(far2Split.at(0).toDouble(), far2Split.at(1).toDouble());

    Crossing *crossing = new Crossing(id);
    crossing->addPointToPolyline(p1);
    crossing->addPointToPolyline(p2);
    crossing->addPointToPolyline(p3);
    crossing->addPointToPolyline(p4);
    geospatialIndex_->insert(crossing);
    return true;
}

bool FileReader::createTrafficSignal(QVariantMap properties) {
    unsigned long hexId = 0, nodeId = 0;
    if (!properties.contains(iSimParse::PARSE_KEYWORD_PROP_HEXID)) {
        emit announceLog(tr("Traffic Signal does not have a valid hex id"));
        return false;
    }
    hexId = properties[iSimParse::PARSE_KEYWORD_PROP_HEXID].toString().toLong(0, 16);

    if (!properties.contains(iSimParse::PARSE_KEYWORD_PROP_NODE)) {
        emit announceLog(tr("Traffic Signal %1 does not have a valid node id").arg(hexId));
        return false;
    }
    nodeId = properties[iSimParse::PARSE_KEYWORD_PROP_NODE].toString().toLong(0, 16);

    if (!properties.contains(iSimParse::PARSE_KEYWORD_PROP_PHASE)) {
        emit announceLog(tr("Traffic Signal %1 does not have a valid phase list").arg(hexId));
        return false;
    }

    TrafficSignal *trafficSignal = new TrafficSignal(hexId, nodeId);

    QVariantList phases = properties[iSimParse::PARSE_KEYWORD_PROP_PHASE].toList();
    foreach (QVariant phaseVariant, phases) {
        QVariantMap phaseMap = phaseVariant.toMap();
        QString name = phaseMap["name"].toString();
        TrafficPhase* phase = new TrafficPhase(name);

        //the list of segments contains the to & from info, along the color
        foreach (QVariant segment, phaseMap["segments"].toList()) {
            QVariantMap segmentMap = segment.toMap();
            unsigned long fromSegId = segmentMap["segment_from"].toString().toLong(0, 16);
            unsigned long toSegId = segmentMap["segment_to"].toString().toLong(0, 16);
            phase->segments.append(QPair<unsigned long, unsigned long>(fromSegId, toSegId));
        }

        foreach (QVariant crossing, phaseMap["crossings"].toList()) {
            QVariantMap crossingMap = crossing.toMap();
            unsigned long crossingId = crossingMap["id"].toString().toLong(0, 16);
            phase->crossings.append(crossingId);
        }

        trafficSignal->addPhase(name, phase);
    }

    geospatialIndex_->insert(trafficSignal);
    return true;
}

bool FileReader::createPhaseData(QVariantMap properties) {
    unsigned long hexId = 0;
    unsigned int tick = 0;
    QString currentPhase = "A";

    if (!properties.contains(iSimParse::PARSE_KEYWORD_PROP_HEXID)) {
        emit announceLog(tr("Phase Data does not have a valid hex id"));
        return false;
    }
    hexId = properties[iSimParse::PARSE_KEYWORD_PROP_HEXID].toString().toLong(0, 16);

    if (!properties.contains(iSimParse::PARSE_KEYWORD_PROP_FRAME)) {
        emit announceLog(tr("Phase Data %1 does not have a valid frame").arg(hexId));
        return false;
    }
    tick = properties[iSimParse::PARSE_KEYWORD_PROP_FRAME].toInt();

    if (!properties.contains(iSimParse::PARSE_KEYWORD_PROP_CURRPHASE)) {
        emit announceLog(tr("Phase Data %1 does not have a valid current phase").arg(hexId));
        return false;
    }
    currentPhase = properties[iSimParse::PARSE_KEYWORD_PROP_CURRPHASE].toString();

    QVariantList phases = properties[iSimParse::PARSE_KEYWORD_PROP_PHASE].toList();
    foreach (QVariant phaseVariant, phases) {
        QVariantMap phaseMap = phaseVariant.toMap();
        QString name = phaseMap["name"].toString();
        if (name == currentPhase) {

            TrafficPhaseData* trafficPhaseData = new TrafficPhaseData(hexId, tick, name);
            foreach (QVariant segment, phaseMap["segments"].toList()) {
                QVariantMap segmentMap = segment.toMap();
                //unsigned long fromSegId = segmentMap["segment_from"].toString().toLong(0, 16);
                //unsigned long toSegId = segmentMap["segment_to"].toString().toLong(0, 16);
                // Asume that the order of pair is the same in trafficsignal
                unsigned int color = segmentMap["current_color"].toString().toInt();
                trafficPhaseData->colors.append(color);
            }
            temporalIndex_->insert(trafficPhaseData);

            foreach (QVariant crossing, phaseMap["crossings"].toList()) {
                QVariantMap crossingMap = crossing.toMap();
                unsigned long crossingId = crossingMap["id"].toString().toLong(0, 16);
                unsigned int color = crossingMap["current_color"].toString().toInt();
                CrossingPhaseData* crossingPhaseData = new CrossingPhaseData(tick, crossingId, color);
                temporalIndex_->insert(crossingPhaseData);
            }
            break;
        }
    }

    return true;
}
