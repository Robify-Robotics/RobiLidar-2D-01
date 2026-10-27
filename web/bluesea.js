/***********Global variable descriptions start ***********************/

const pointsData = [[1, 1]];//Lidar point cloud data
var option;//echart.js configuration parameters
var dom = document.getElementById('container');//Plot container
var myChart;//Global echart component variable
var size = 1;//Displayed point size
var isPlay = true;//Control whether lidar point cloud refresh is stopped
var isPlay2 = true;//Control lidar list refresh
var currentTime = 1;//Test time
var currentTime1 = 1;//Test time
var type = "uart";//Lidar type
var SDKENV = "other";//Operating system running the SDK service: Windows|Linux
var scale = 0.32; //Set the ratio between rectangle pixels and actual coordinates   320/1000
var isDrag = false;//Protection zone drag state
var isClick = false;//Whether a protection zone is clicked
//Protection zone global variables
var echartL = 0;
var echartT = 0;
var echartR = 0;
var echartB = 0;
var zoneSerial = 0;//Current protection zone index
var outputID = 0;//Protection zone type
var zoneID = 0;//ID of the added drawn protection zone, also the total count
//
var zoneColor1 = "#333";//Triggered observation zone color #800000
var zoneColor2 = "#333";//Triggered warning zone color
var zoneColor3 = "#333";//Triggered alarm zone color

var zone_flag = 0;//Type of returned protection zone information
var zone_events = 0;//Detailed information returned by the protection zone
var zone_actived = 0;//Active protection zone
var zone_text = "Status information:";

var colorList = ['#E5040F', '0FFA0F', '#F6931C', '#FFD52E']
var offsetX = 0;
var offsetY = 0;
var zoneList = [];

/***********Global variable descriptions  end ***********************/
//Initialize echart.js
myChart = echarts.init(dom, null, {
  renderer: 'canvas',
  useDirtyRect: false
});

//Periodically retrieve point cloud data from the server
function getPointsData() {
  isPlay = true;
  $.get("/data", function (data, status) {
    if (isPlay)
      getPointsData();
    if (status != "success")
      return;
    var jsondatas = eval("(" + data + ")");
    $.each(jsondatas.data, function (i, n) {
      pointsData[i] = [n.distance * 1000, n.angle];
      //console.log("dis:" + n.distance + "  angle:" + n.angle);
    })
    option.series[0].symbolSize = size;
    option.series[0].data = pointsData;
    $('#index').attr("value", jsondatas.N);
    if (type != "uart") {
        if(zone_flag!=jsondatas.zone_flag ||zone_events!=jsondatas.zone_events ||zone_actived!=jsondatas.zone_actived)
        {
            zone_flag = jsondatas.zone_flag;
            zone_events = jsondatas.zone_events;
            zone_actived = jsondatas.zone_actived;
            ShowZoneMSG(zone_flag, zone_events, zone_actived);
        }
    }
    $('#timestamp_s').attr("value", jsondatas.timestamp_s);
    $('#timestamp_us').attr("value", jsondatas.timestamp_us);

    myChart.setOption(option, false);


  });
};
//Stop retrieving lidar data
function stopGetData() {
  isPlay = false;
}
//Control lidar motion state
function action(id) {
  $("#" + id).attr("class", "layui-btn layui-btn-primary");
  var cmd = "/action?cmd=" + id;
  $.get(cmd, function (data, status) {
    if (status = "success") {
      $("#" + id).attr("class", "layui-btn layui-btn-radius");
      console.log("id:" + id + "cmd:" + cmd);
      console.log(data);
    }
  })
}

//Get device information
function getDevInfo() {
  //Leaving the timer running causes requests to block
  isPlay = false;
  $.get("/getDevinfo", function (data, status) {
    var jsondatas = eval("(" + data + ")").data;
    if (type == "udp" || type == "vpc") {
      for (var val in jsondatas) {
        //alert(val + " " + jsondatas[val]);//Example output:name 
        $("#" + val).attr("value", jsondatas[val]);
        if (val == "ERR") {
          var tmp = jsondatas[val];
          $("#set" + val).attr("value", tmp[0]);
        } else if (val == "PNP" || val == "SMT" || val == "DSW" || val == "ATS" || val == "TFX" || val == "PST") {
          var tmp = jsondatas[val];
          $('#set' + val).find("option[value=" + tmp + "]").attr("selected", true);

        }
        else {
          $("#set" + val).attr("value", jsondatas[val]);
        }
      }
      layui.form.render('select') //Render again
    }
    //The uart version only has a serial number
    else if (type == "uart") {
      $("#UID").attr("value", jsondatas.UID);
    }
    getPointsData();
  });
}
function ipStr(str) {
  var arr1 = str.split(".");
  let result = arr1[0].toString().padStart(3, '0') + "." + arr1[1].toString().padStart(3, '0') + "." + arr1[2].toString().padStart(3, '0') + "." + arr1[3].toString().padStart(3, '0');
  return result;

}

var g_num = 0;
//Set information for the specified device
function setDevInfo() {
  //Get current device information first, then compare
  var value;
  var value1;
  //Check all options for modified data
  for (var index = 0; index <= 16; index++) {
    if (index == 0) {
      value = $("#setRPM").val();
      value1 = $("#RPM").val();
      if (value == value1)
        continue;
    } else if (index == 1) {
      value = $("#setERR").val();
      var tmp = $("#ERR").val();
      var arr1 = tmp.split(",");
      var value1 = arr1[0];
      if (value == value1)
        continue;
    } else if (index == 2) {
      var valuetmp1 = $("#setIPv4").val();
      var valuetmp2 = $("#setmask").val();
      var valuetmp3 = $("#setgateway").val();
      var valuetmp4 = $("#setlocal_port").val();

      var value1tmp1 = $("#IPv4").val();
      var value1tmp2 = $("#mask").val();
      var value1tmp3 = $("#gateway").val();
      var value1tmp4 = $("#local_port").val();
      if (valuetmp1 == value1tmp1 && valuetmp2 == value1tmp2 && valuetmp3 == value1tmp3 && valuetmp4 == value1tmp4)
        continue;
      value = ipStr(valuetmp1) + " " + ipStr(valuetmp2) + " " + ipStr(valuetmp3) + " " + valuetmp4.toString().padStart(5, '0');

    } else if (index == 3) {
      var valuetmp1 = $("#setsrv_ip").val();
      var valuetmp2 = $("#setsrv_port").val();
      var value1tmp1 = $("#srv_ip").val();
      var value1tmp2 = $("#srv_port").val();
      if (valuetmp1 == value1tmp1 && valuetmp2 == value1tmp2)
        continue;
      value = ipStr(valuetmp1) + " " + valuetmp2.toString().padStart(5, '0');
    } else if (index == 4) {
      value = $("#setNSP").val();
      value1 = $("#NSP").val();
      if (value == value1)
        continue;
    } else if (index == 5) {
      value = $("#setUID").val();
      value1 = $("#UID").val();
      if (value == value1)
        continue;
    } else if (index == 6) {
      value = $("#setFIR").val();
      value1 = $("#FIR").val();
      if (value == value1)
        continue;
    } else if (index == 7) {
      continue;
      // value = $("#setPUL").val();
      // value1 = $("#PUL").val();
      // if(value==value1)
      //   continue;
    } else if (index == 8) {
      continue;
      //value = $("#setVER").val();
    } else if (index == 9) {
      value = $("#setIO").val();
      value1 = $("#IO").val();
      if (value == value1)
        continue;
    } else if (index == 10) {
      value = $("#setSMT").val();
      value1 = $("#SMT").val();
      if (value == value1)
        continue;
    } else if (index == 11) {
      value = $("#setDSW").val();
      value1 = $("#DSW").val();
      if (value == value1)
        continue;
    }
    else if (index == 12) {
      value = $("#setDID").val();
      value1 = $("#DID").val();
      if (value == value1)
        continue;
    } else if (index == 13) {
      value = $("#setATS").val();
      value1 = $("#ATS").val();
      if (value == value1)
        continue;
    } else if (index == 14) {
      value = $("#setTFX").val();
      value1 = $("#TFX").val();
      if (value == value1)
        continue;
    } else if (index == 15) {
      value = $("#setPST").val();
      value1 = $("#PST").val();
      if (value == value1)
        continue;
    } else if (index == 16) {
      value = $("#setAF").val();
      value1 = $("#AF").val();
      if (value == value1)
        continue;
    } else {
      return;
    }
    var cmd = "/setDevinfo?index=" + index + "&value=" + value;
    $.get(cmd, function (data, status) {
      alert("cmd:" + cmd + "  " + data);
      getDevInfo();
    })
  }
}
function showClientTable() {
  layui.use('table', function () {
    var table = layui.table;

    $.get("/api/stats", function (data, status) {
      var jsondatas = eval("(" + data + ")");
      if (jsondatas.result != "SUCCESS")
        return;
      layer.open({
        type: 1,
        area: ["700px", '500px'],
        title: "Current connection information",
        maxmin: false,
        content: '<div><table id="templateTable2" lay-filter="templateTable2"></table></div>', //Define a div for the data table first
        success: function (index, layero) {
          table.render({
            elem: '#templateTable2'
            , height: 420
            , width: '100%'
            , data: jsondatas.data
            , cols: [[ //Set data table headers
              { field: 'ID', title: 'Internal ID', width: 100, sort: true, fixed: 'left' }
              , { field: 'type', title: 'Type', fixed: 'left' }
              , { field: 'state', title: 'State', width: 150, sort: true, fixed: 'left' }
              , { field: 'loc', title: 'Local address', width: 200, fixed: 'left' }
              , { field: 'rem', title: 'Destination address', width: 200, fixed: 'left' }
            ]]
            , page: true //Enable pagination
            , limits: [10, 20, 30, 40, 50, 60, 70, 80, 90, 100] //Dynamically set the number of records per page
            , limit: 10 //Default number of records per page
            , response: { // Response data format
              statusName: 'result'
              , statusCode: "SUCCESS"
            }
          });
        }

      });
    });
  });
}

function showLidarTable() {
  layui.use('table', function () {
    var table = layui.table;
    $.get("getLidarList?mode=0", function (data, status) {
      var jsondatas = eval("(" + data + ")");
      if (jsondatas.result != "SUCCESS")
        return;
      layer.open({
        type: 1,
        area: ["700px", '500px'],
        title: "Currently available lidar devices",
        maxmin: false,
        content: '<div><table id="templateTable" lay-filter="templateTable"}></table></div>', //Define a div for the data table first
        success: function (index, layero) {
          table.render({
            elem: '#templateTable'
            , height: 420
            , width: '100%'
            , data: jsondatas.data
            , cols: [[ //Set data table headers
              { field: 'type', title: 'Device type', width: 100, sort: true, fixed: 'left' }
              , { field: 'com_port', title: 'Serial port', width: 100, fixed: 'left' }
              //, { field: 'com_speed', title: 'Baud rate', width: 100, fixed: 'left' }
              , { field: 'conn_ip', title: 'IP address', width: 150, fixed: 'left' }
              , { field: 'conn_port', title: 'IP port', width: 100, fixed: 'left' }
              , { field: 'time', title: 'Update time', width: 100, fixed: 'left' }
            ]]
            , page: true //Enable pagination
            , limits: [10, 20, 30, 40, 50, 60, 70, 80, 90, 100] //Dynamically set the number of records per page
            , limit: 10 //Default number of records per page
            , response: { // Response data format
              statusName: 'result'
              , statusCode: "SUCCESS"
            }
          });
          var time = setInterval(function () {
            table.reload('templateTable', {
              url: '/getLidarList?mode=0'
              , where: {} //Set additional parameters for the asynchronous data API
              , page: {
                curr: 1 //Restart from page 1
              }
            });
          }, 3000);
        },
        cancel: function () {
          $.get("getLidarList?mode=1", function (data, status) {
            var jsondatas = eval("(" + data + ")");
            if (jsondatas.result != "SUCCESS")
              return;
          })
        }
      });
    });
  });
}
//Initialize by retrieving the lidar type
$(document).ready(function () {
  $.get("/init", function (data, status) {
    var jsondatas = eval("(" + data + ")");
    type = jsondatas.data[0].type;
    SDKENV = jsondatas.data[0].SDKENV;
  });
  getDevInfo();
});

/***********Polar plot parameter configuration start ********************************/
option = {
  title: {
    text: 'Lidar plot'
  },
  legend: {
    data: ['Lidar point cloud data', 'Protection zone-Observation zone', 'Protection zone-Warning zone', 'Protection zone-Alarm zone'],
    left: 'right',
  },
  polar: {
    center: ["60%", "60%"]
  },
  angleAxis: {
    type: 'value',
    boundaryGap: false,
    startAngle: 0,
    min: 0,
    max: 360,
    interval: 30,
    splitLine: {
      show: true
    },
    axisLine: {
      show: false
    }
  },
  radiusAxis: {
    type: 'value',
    min: 0,//'dataMin',   Using the received data for the display range causes excessive fluctuation and makes observation difficult
    max: 5000,//'dataMax',
    axisLine: {
      show: false
    },
    axisLabel: {
      rotate: 45
    }
  },
  myarea: {
    silent: true
  },
  graphic: [
    {
      id: 'text4',
      type: 'text',
      $action: 'merge',
      //z: 100,
      left: 100,
      top: 100,
      style: {
        fill: '#333',
        text: [
          zone_text
        ].join('\n'),
        font: '20px Microsoft YaHei',
      }
    }
  ],
  series: [
    {
      id: 0,
      name: 'Lidar point cloud data',
      type: 'scatter',
      coordinateSystem: 'polar',
      showAllSymbol: false,
      large: true,
      largeThreshold: 10,
      symbolSize: 2,
      data: pointsData,
      color: 'green'
    }
    ,
    {
      id: 10,
      name: 'Protection zone-Observation zone',
      type: 'scatter',
      coordinateSystem: '',
      symbolSize: 2,
      color: 'blue'
    }
    ,
    {
      id: 20,
      name: 'Protection zone-Warning zone',
      type: 'scatter',
      coordinateSystem: '',
      symbolSize: 2,
      color: 'yellow'
    },
    {
      id: 30,
      name: 'Protection zone-Alarm zone',
      type: 'scatter',
      coordinateSystem: '',
      symbolSize: 2,
      color: 'red'
    }
  ]
};
/***********Polar plot parameter configuration end ********************************/
if (option && typeof option === 'object') {
  myChart.setOption(option, true);
}
/***********Listen for mouse wheel events in the current plot container and set the maximum coordinate value********************************/
if (dom.addEventListener) {
  // IE9, Chrome, Safari, Opera
  dom.addEventListener("mousewheel", MouseWheelHandler, false);
  // Firefox
  dom.addEventListener("DOMMouseScroll", MouseWheelHandler, false);
}

function MouseWheelHandler(e) {
  var e = window.event || e; // old IE support
  var delta = Math.max(-1, Math.min(1, (e.wheelDelta || -e.detail)));
  var tmp = option.radiusAxis.max + delta * 100;
  if (tmp < 500)
    return;
  option.radiusAxis.max = tmp;

  var old_scale = scale;
  scale = 320 / tmp * 1.00;
  //Scale the current protection zone shapes with mouse zoom (rectangles and sectors only)
  var jsonarray = eval(option.graphic);
  for (var i = 0; i < zoneID; i++) {
    if (jsonarray[i + 1].type == 'rect') {
      jsonarray[i + 1].shape.x = 480 + (jsonarray[i + 1].shape.x - 480) / old_scale * scale
      jsonarray[i + 1].shape.y = 480 + (jsonarray[i + 1].shape.y - 480) / old_scale * scale
      jsonarray[i + 1].shape.width = (jsonarray[i + 1].shape.width) / old_scale * scale
      jsonarray[i + 1].shape.height = (jsonarray[i + 1].shape.height) / old_scale * scale
    } else if (jsonarray[i + 1].type == 'sector') {
      jsonarray[i + 1].shape.r = (jsonarray[i + 1].shape.r) / old_scale * scale
      jsonarray[i + 1].shape.r0 = (jsonarray[i + 1].shape.r0) / old_scale * scale
    }
  }
  option.graphic = jsonarray;
  myChart.setOption(option, false);

  //console.log(tmp);
  //Keep lidar plot wheel behavior independent of page scrolling
  window.event.preventDefault();

}
/***********Listen for mouse wheel events in the current plot container and set the maximum coordinate value********************************/

layui.use(['element', 'layer', 'form', 'table'], function () {
  var element = layui.element, //Tab switching and switch event listeners require the element module
    layer = layui.layer,
    form = layui.form;
  //Listen to all forms
  form.on('select(size)', function (data) {
    size = $("select[id=selectSize").val();
    console.log(size);
    form.render('select');//select is a literal argument, not a selector
  });
  form.on('select(zone)', function (data) {
    var tmp = $("select[id=setZoneGraphics").val();
    if (tmp == 'rect') {
      $("#valueL").html("Left");
      $("#valueT").html("Top");
      $("#valueR").html("Right");
      $("#valueB").html("Bottom");
    } else if (tmp == 'sector') {
      $("#valueL").html("Start angle");
      $("#valueT").html("End angle");
      $("#valueR").html("Far distance");
      $("#valueB").html("Near distance");
    }

    form.render('select');//select is a literal argument, not a selector
  });
  form.verify({
    require: function (value) {
      value = $.trim(value);
      if (!value) {
        return "Required fields cannot be empty";
      }
    },
    ip: [
      /^(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])\.(\d{1,2}|1\d\d|2[0-4]\d|25[0-5])$/
      , 'Invalid IP address format'
    ]
  });
});
/***************************************Protection zone functions***********************************/
function readZone() {

    ClearZone();
  $.get("/getZone", function (data, status) {
    var jsondatas = eval("(" + data + ")");
    var N = jsondatas.data.N;
    if (N <= 0) return;

    for (var i = 0; i < N; i++) {
      var type = jsondatas.data.values[i].type;
      if (type != 2 && type != 4) return;
      if (type == 2) { ZoneGraphics = 'rect'; }
      if (type == 4) { ZoneGraphics = 'sector'; }

      zoneSerial = jsondatas.data.values[i].zoneID;
      //Do not display if the configured protection zone differs from the zone information in the returned data
      if( $("#setZone").val()!=zoneSerial)
          return;
      outputID = jsondatas.data.values[i].outputID;
      echartL = jsondatas.data.values[i].value[0];
      echartT = jsondatas.data.values[i].value[1];
      echartR = jsondatas.data.values[i].value[2];
      echartB = jsondatas.data.values[i].value[3];
      drawZone(ZoneGraphics, zoneSerial, echartL, echartT, echartR, echartB, outputID);
    }
  });
}

function writeZone() {
  debugger
  $.post("/setZone", JSON.stringify(zoneList), function (data, status) {
    debugger
  }, "json");
}
function ShowZone() {
  var ZoneGraphics = $("#setZoneGraphics").val();
  //Different protection zone levels use different colors
  outputID = $("#setZoneAlarm").val();
  zoneSerial = $("#setZone").val();
  echartL = $("#zoneL").val();
  echartT = $("#zoneT").val();
  echartR = $("#zoneR").val();
  echartB = $("#zoneB").val();

  drawZone(ZoneGraphics, zoneSerial, echartL, echartT, echartR, echartB, outputID);
}

function drawZone(ZoneGraphics, zoneSerial, echartL, echartT, echartR, echartB, outputID) {
  $("#setZone").attr("value", zoneSerial);
  if (outputID == '0')
    ZoneAlarm = 'blue';
  else if (outputID == '1')
    ZoneAlarm = 'yellow';
  else if (outputID == '2')
    ZoneAlarm = 'red';

  var type = 0;
  var jsonarray = eval(option.graphic);
  var arr;
  if (ZoneGraphics == 'rect') {
    type = 2;
    arr =
    {
      type: ZoneGraphics,
      id: zoneID,
      $action: 'merge',
      invisible: false,
      draggable: false,
      silent: false,
      shape:
      {
         x: 480 + echartL * 1 * scale,
         y: 480 + echartT * 1 * scale,
         width: (echartR - echartL) * scale,
         height: (echartT - echartB) * -1 * scale
        //Here 90 degrees is treated as 0 degrees, based on
        //x: 480 + echartT * 1 * scale,
        //y: 480 - echartL * 1 * scale,
        //width: (echartT - echartB) * -1 * scale,
        //height: -(echartR - echartL) * scale,
      },
      style: {
        fill: '#fff0',
        stroke: ZoneAlarm
      }
    }
  } else if (ZoneGraphics == 'sector') {
    type = 4;
    arr =
    {
      type: ZoneGraphics,
      id: zoneID,
      // top: 480,
      // button:480,
      // right: 480,
      // left:480,
      $action: 'merge',
      invisible: false,
      draggable: false,
      silent: false,
      shape:
      {
        cx: 480,
        cy: 480,
        r: echartR * 1 * scale,
        r0: echartB * 1 * scale,
        //startAngle: -Math.PI / 2 + echartL / 1800 * Math.PI,
        //endAngle: -Math.PI / 2 + echartT / 1800 * Math.PI，
        startAngle:  echartL / 1800 * Math.PI,
        endAngle:  echartT / 1800 * Math.PI,
        clockwise: true
      },
      style: {
        fill: '#fff0',
        stroke: ZoneAlarm
      }
    }
  } else {
    return -1;
  }
  zoneID++;
  jsonarray.push(arr);
  option.graphic = jsonarray;
  myChart.setOption(option, false);

  //Collect the protection zone boundaries to save and store them in an array
  var zone = {};
  zone['type'] = type;
  zone['zoneID'] = $("#setZone").val() * 1;
  zone['outputID'] = outputID * 1;
  zone['arg1'] = echartL * 1;
  zone['arg2'] = echartT * 1;
  zone['arg3'] = echartR * 1;
  zone['arg4'] = echartB * 1;
  zoneList.push(zone);
  return 0;
}


function ClearZone() {
  for (var i = 0; i < zoneID; i++) {
    option.graphic[1 + i].$action = 'remove';
  }
  zoneID = 0;
  zoneList = [];
  myChart.setOption(option, false);
  var jsonarray = eval(option.graphic);
  var tmp = [];
  tmp.push(jsonarray[0]);
  option.graphic = tmp;
}
function getbit(x, y) {
  return ((x) >> (y) & 1);
}
function ShowZoneMSG(flag, event, active) {


  var text = 'Protection zone:' + active + '\n';
  if (flag % 2 == 1) {
    //Error information
    if (getbit(event, 0) == 1) {
      text += 'Insufficient power supply\n'
    }
    if (getbit(event, 1) == 1) {
      text += 'Motor stalled\n'
    }
    if (getbit(event, 2) == 1) {
      text += 'Ranging module overtemperature\n'
    }
    if (getbit(event, 3) == 1) {
      text += 'Network error\n'
    }
    if (getbit(event, 4) == 1) {
      text += 'No output from ranging module\n'
    }
  }
  if (flag >= 0x100) {
    //Protection zone information
    if (getbit(event, 12) == 1) {
      text += 'Observation！！！\n'
    }
    if (getbit(event, 13) == 1) {
      text += 'Warning！！！\n'
    }
    if (getbit(event, 14) == 1) {
      text += 'Alarm！！！\n'
    }
    if (getbit(event, 15) == 1) {
      text += 'Obstruction！\n'
    }
    if (getbit(event, 16) == 1) {
      text += 'No data\n'
    }
    if (getbit(event, 17) == 1) {
      text += 'No protection zone configured\n'
    }
    if (getbit(event, 18) == 1) {
      text += 'Internal system error\n'
    }
    if (getbit(event, 19) == 1) {
      text += 'Abnormal system operation\n'
    }
    if (getbit(event, 20) == 1) {
      //Duplicates the fourth item above; disabled here
      //text+='Network error\n'
    }
    if (getbit(event, 21) == 1) {
      text += 'Device updating\n'
    }
    if (getbit(event, 22) == 1) {
      text += 'Zero-position output\n'
    }
  }
  console.log(text);
  option.graphic[0].style.fill = '#FF3727';
  option.graphic[0].style.text = text;
}
