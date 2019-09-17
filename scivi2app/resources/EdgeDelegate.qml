import QtQuick 2.10
import QtQuick.Shapes 1.0
import scivi 0.1

EdgeView {
    id: root

    property double clickMargin: Qt.platform.os === "Android" ? -10 : -6.0

    Item {
        id: srcPortal

        Rectangle {
            id: srcPortalNode
            x: srcPoint.x + 26
            y: srcPoint.y - width/2
            width: 14
            height: 14
            border.color: 'black'
            color: '#d6d6d6'
            Text {
                anchors.centerIn: parent
                text: root.edge.number
            }

            MouseArea {
                anchors.fill: parent
                anchors.margins: clickMargin
                acceptedButtons: Qt.RightButton
                onClicked: root.edge.number = -1
            }
        }

        Shape {
            ShapePath {
                strokeWidth: 2
                strokeColor: root.edge.color
                capStyle: ShapePath.FlatCap
                startX: srcPoint.x
                startY: srcPoint.y
                PathLine {
                    x: srcPortalNode.x
                    y: srcPortalNode.y + srcPortalNode.height / 2
                }
            }
        }
    }

    Item {
        id: destPortal

        Rectangle {
            id: destPortalNode
            x: destPoint.x - 40
            y: destPoint.y - width/2
            width: 14
            height: 14
            border.color: 'black'
            color: '#d6d6d6'
            Text {
                anchors.centerIn: parent
                text: root.edge.number
            }

            MouseArea {
                anchors.fill: parent
                anchors.margins: clickMargin
                acceptedButtons: Qt.RightButton
                onClicked: root.edge.number = -1
            }
        }

        Shape {
            ShapePath {
                strokeWidth: 2
                strokeColor: root.edge.color
                capStyle: ShapePath.FlatCap
                startX: destPoint.x
                startY: destPoint.y
                PathLine {
                    x: destPortalNode.x + destPortalNode.width
                    y: destPortalNode.y + destPortalNode.height / 2
                }
            }
        }
    }

    Shape {
        id: shape
        ShapePath {
            id: shapePath
            strokeWidth: 2
            strokeColor: root.edge.color
            capStyle: ShapePath.FlatCap
            fillColor: "transparent"
            startX: srcPoint.x; startY: srcPoint.y
            PathCubic {
                id: pathCubic
                x: destPoint.x
                y: destPoint.y
                control1X: c1.x
                control1Y: c1.y
                control2X: c2.x
                control2Y: c2.y
            }
        }
    }

    function getCurveBounds(ax, ay, bx, by, cx, cy, dx, dy)
    {
        var px, py, qx, qy, rx, ry, sx, sy, tx, ty,
                tobx, toby, tocx, tocy, todx, tody, toqx, toqy, torx, tory, totx, toty;
        var x, y, minx, miny, maxx, maxy;

        minx = miny = Number.POSITIVE_INFINITY;
        maxx = maxy = Number.NEGATIVE_INFINITY;

        tobx = bx - ax;  toby = by - ay;  // directions
        tocx = cx - bx;  tocy = cy - by;
        todx = dx - cx;  tody = dy - cy;
        var step = 1/40;      // precission
        for(var d=0; d<1.001; d+=step)
        {
            px = ax +d*tobx;  py = ay +d*toby;
            qx = bx +d*tocx;  qy = by +d*tocy;
            rx = cx +d*todx;  ry = cy +d*tody;
            toqx = qx - px;      toqy = qy - py;
            torx = rx - qx;      tory = ry - qy;

            sx = px +d*toqx;  sy = py +d*toqy;
            tx = qx +d*torx;  ty = qy +d*tory;
            totx = tx - sx;   toty = ty - sy;

            x = sx + d*totx;  y = sy + d*toty;
            minx = Math.min(minx, x); miny = Math.min(miny, y);
            maxx = Math.max(maxx, x); maxy = Math.max(maxy, y);
        }
        return {x:minx, y:miny, width:maxx-minx, height:maxy-miny};
    }

    function checkClicked(ax, ay, bx, by, cx, cy, dx, dy, mx, my) {
        var px, py, qx, qy, rx, ry, sx, sy, tx, ty,
                tobx, toby, tocx, tocy, todx, tody, toqx, toqy, torx, tory, totx, toty;
        var x, y, minDistance;
        var distance;
        minDistance = Number.POSITIVE_INFINITY;

        tobx = bx - ax;  toby = by - ay;  // directions
        tocx = cx - bx;  tocy = cy - by;
        todx = dx - cx;  tody = dy - cy;
        var step = 1/40;      // precission
        for(var d=0; d<1.001; d+=step)
        {
            px = ax +d*tobx;  py = ay +d*toby;
            qx = bx +d*tocx;  qy = by +d*tocy;
            rx = cx +d*todx;  ry = cy +d*tody;
            toqx = qx - px;      toqy = qy - py;
            torx = rx - qx;      tory = ry - qy;

            sx = px +d*toqx;  sy = py +d*toqy;
            tx = qx +d*torx;  ty = qy +d*tory;
            totx = tx - sx;   toty = ty - sy;

            x = sx + d*totx;  y = sy + d*toty;
            distance = Math.sqrt((x-mx)*(x-mx)+(y-my)*(y-my));
            if (distance < minDistance) {
                minDistance = distance;
            }
        }
        var threshold = 8.0;
        console.log('Distance', minDistance)
        return minDistance < threshold;
    }

    Rectangle {
        id: mouseAreaOverlay
        x: getCurveBounds(srcPoint.x, srcPoint.y, c1.x, c1.y, c2.x, c2.y, destPoint.x, destPoint.y).x
        y: getCurveBounds(srcPoint.x, srcPoint.y, c1.x, c1.y, c2.x, c2.y, destPoint.x, destPoint.y).y
        width: getCurveBounds(srcPoint.x, srcPoint.y, c1.x, c1.y, c2.x, c2.y, destPoint.x, destPoint.y).width
        height: getCurveBounds(srcPoint.x, srcPoint.y, c1.x, c1.y, c2.x, c2.y, destPoint.x, destPoint.y).height
        //        color: '#10ff0000'
        color: 'transparent'
        enabled: shape.visible
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            propagateComposedEvents: true
            preventStealing: false
            onClicked: {
                var mX = mouse.x
                var mY = mouse.y
                var p0x = srcPoint.x
                var p0y = srcPoint.y
                var p1x = c1.x
                var p1y = c1.y
                var p2x = c2.x
                var p2y = c2.y
                var p3x = destPoint.x
                var p3y = destPoint.y

                var mousePos = mouseAreaOverlay.mapToItem(shape, mX, mY)
                mX = mousePos.x
                mY = mousePos.y
                if (checkClicked(srcPoint.x, srcPoint.y, c1.x, c1.y, c2.x, c2.y, destPoint.x, destPoint.y, mX, mY)) {
                    console.log("Cut the edge")
                    root.rightButtonClicked(root)
                }
            }
        }
    }

    states: [
        State {
            name: "normal"
            when: (root.edge.number === -1)
            PropertyChanges {
                target: shape
                visible: true
            }
            PropertyChanges {
                target: srcPortal
                visible: false
            }
            PropertyChanges {
                target: destPortal
                visible: false
            }
        },
        State {
            name: "collapsed"
            when: (root.edge.number !== -1)
            PropertyChanges {
                target: shape
                visible: false
            }
            PropertyChanges {
                target: srcPortal
                visible: true
            }
            PropertyChanges {
                target: destPortal
                visible: true
            }
        }
    ]
}
