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

void FileReader::setTarget(QString path, iSimGUI::DataType type, bool useDB) {
    path_ = path;
    type_ = type;
    useDB_ = useDB;
    isSpatialDataFinished_ = false;
    isTemporalDataDetected_ = false;
    if (fileHandle_) {
        fileHandle_->close();
        delete fileHandle_;
    }
    fileHandle_ = new QFile(path_);
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
        emit announceStatus(tr("The file is loaded successfully."));
        emit announceLog("The file is loaded successfully.");
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
        bool isSegmentState = (type_ == iSimGUI::DATA_MEDIUM_TERM && objType == iSimParse::PARSE_KEYWORD_STATE_SEGMENT);
        bool isAgent = (type_ == iSimGUI::DATA_SHORT_TERM && (objType == iSimParse::PARSE_KEYWORD_STATE_DRIVER ||
                                                     objType == iSimParse::PARSE_KEYWORD_STATE_BUSDRIVER ||
                                                     objType == iSimParse::PARSE_KEYWORD_STATE_PEDESTRIAN ||
                                                     objType == iSimParse::PARSE_KEYWORD_STATE_PASSENGER));
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
    }
    return true;
}

bool FileReader::createMesoscopic(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    // ("segmentState",0,0x37dc2d0,{"speed":"1388.8889","flow":"0","density":"0"})

    Mesoscopic mesoscopic;
    mesoscopic.setSegmentID(objID);
    mesoscopic.setTick(frameID);

    // Get speed
    QMap<QString, QString>::iterator propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_SPEED);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Mesoscopic %1 does not have speed value").arg(objID));
        return false;
    }
    mesoscopic.setSpeed(QString(propertiesIter.value()).toDouble());

    // Get flow
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_FLOW);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Mesoscopic %1 does not have flow value").arg(objID));
        return false;
    }
    mesoscopic.setFlow(QString(propertiesIter.value()).toDouble());

    // Get density
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_DENSITY);
    if (propertiesIter==properties.end()) {
        emit announceLog(tr("Mesoscopic %1 data does not have density value").arg(objID));
        return false;
    }
    mesoscopic.setDensity(QString(propertiesIter.value()).toDouble());

    temporalIndex_->insertMesoscopicData(mesoscopic);
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
    //("Driver",11,47,{"xPos":"37271822","yPos":"14399409","angle":"128.89281","length":"400","width":"200","info":"MLC","mandatory":"0"})

   return true;
}

bool FileReader::createBusDriver(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    return true;
}

bool FileReader::createPedestrian(unsigned long objID, unsigned int frameID, QMap<QString, QString> &properties) {
    return true;
}

bool FileReader::parseRoadNetworkDataLine(const QString& objType, unsigned long objID, QMap<QString, QString> &properties) {
    // Handle uni nodes and multi nodes similarly

    // Enqueue to uninode raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_MULTINODE) return createMultiNode(objID, properties);
    // Enqueue to uninode raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_UNINODE) return createUniNode(objID, properties);
    /*
    // Enqueue to link raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_LINK) return createLink(objID, properties);
    // Enqueue to road segments raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_ROADSEGMENT ) return createRoadSegment(objID, properties);
    // Enqueue to polylines raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_POLYLINE ) return createPolyline(properties);
    // Enqueue to lanes raw data queue
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_LANE ) return createLane(properties);

    if (objType==iSimParse::PARSE_KEYWORD_TYPE_LANECONNECTOR ) return createLaneConnector(properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_GRAPH ) return createGraph(objID, properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_VERTEX ) return createVertex(objID, properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_EDGE ) return createEdge(objID, properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_BUSSTOP) return createBusstop(objID, properties);
    if (objType==iSimParse::PARSE_KEYWORD_TYPE_CROSSING) return createCrossing(objID, properties);
    */
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
        emit announceLog(tr("MultiNode %1 is missing property xPos or yPos or they do not contain integer values").arg(id));
        return false;
    }
    // get aimsunId
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_AIMSUNID);
    if (propertiesIter!=properties.end()) {
        aimsunId = propertiesIter.value().toULong();
    }
    // Create data object
    MultiNode *multiNode = new MultiNode(id, xPos, yPos, aimsunId);
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
        emit announceLog(tr("UniNode %1 is missing property xPos or yPos or they do not contain integer values").arg(id));
        return false;
    }
    // get aimsunId
    propertiesIter = properties.find(iSimParse::PARSE_KEYWORD_PROP_AIMSUNID);
    if (propertiesIter!=properties.end()) {
        aimsunId = propertiesIter.value().toULong();
    }
    // Create data object
    UniNode *uniNode = new UniNode(id, xPos, yPos, aimsunId);
    geospatialIndex_->insert(uniNode);
    return true;
}
