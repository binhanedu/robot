<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>温湿度查询系统</title>
<script type="text/javascript" src="jquery.js"></script>
<script type="text/javascript" src="echarts.min.js"></script>
<style type="text/css">
	*{
		padding: 0px;
		margin: 0px;
		text-decoration:none;
	}
	.wapper_box{
		width: 1000px;
		height:600px;
		margin: 0px auto;
		background-color: aqua;
	}
	.wapper{
		width: 100%;
		display: flex;
		
	}
	.wapper:first-of-type{
		margin-bottom: 16px;
		height: 32px;
		border-bottom:2px solid drakgreen;
/*		background-color: aliceblue;*/
	}
	.wapper h2{
		text-align: center;
		width: 100%;
	}
	.wapper:last-of-type{
		height: 550px;
		background-color: antiquewhite;
		overflow: hidden;
		font-size: 0px;
		
	}
	.wapper .wapper_cell{
		display: inline-block;
		font-size: 14px;
		height: 100%;
	}
	.wapper_cell.left{
		width: 30%;
		background-color: #03e9f47a;
		display: flex;
		flex-direction: column;
	}
	.wapper_cell .wapper_form{
		width: 100%;
		margin: 5px auto 10px;
	}
	.wapper_cell .wapper_table{
		height:488px;
		width: 100%;
	}
	.wapper_title,.wapper_info{
		display: flex;
	}
	.wapper_title{
		margin-bottom:5px;
	}
	.wapper_info{
		height: 458px;
		overflow-y: auto;
	}
	.wapper_info>div{
		font-size: 14px;
		margin-bottom: 3px;
	}
	.wapper_cell .wapper_table .wapper_time{
		width: 155px;
	}
	.wapper_cell .wapper_table .wapper_cell1{
		width: 65px;
	}
	.wapper_cell .wapper_table .wapper_cell2{
		width: 65px;
	}
	
	.wapper_cell.right{
		width: 70%;
		height:550px;
		background-color: pink;
	}
	.wapper_cell .wapper_canvas{
		height: 50%;
		width:100%;
		padding: 12.5px 0px 12.5px 50px;
		box-sizing: border-box;
		background-color: aliceblue;
	}
</style>
<script type="text/javascript">
	function YYYYMMDDstart() {
		MonHead = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
		//先给年下拉框赋内容
		var y = new Date().getFullYear();
		for (var i = (y - 30); i < (y + 30); i++) //以今年为准，前30年，后30年
			document.timesel.YYYY.options.add(new Option(" " + i + " 年", i));

		//赋月份的下拉框
		for (var i = 1; i < 13; i++) {
			if (i < 10){
				document.timesel.MM.options.add(new Option(" 0" + i + " 月", i));
			} else{
				document.timesel.MM.options.add(new Option(" " + i + " 月", i));
			}
		}
		document.timesel.YYYY.value = y;
		document.timesel.MM.value = new Date().getMonth() + 1;
		var n = MonHead[new Date().getMonth()];
		if (new Date().getMonth() == 1 && IsPinYear(YYYYvalue)) n++;
		writeDay(n); //赋日期下拉框Author:meizz
		document.timesel.DD.value = new Date().getDate();
	}

	if (document.attachEvent)
		window.attachEvent("onload", YYYYMMDDstart);
	else
		window.addEventListener('load', YYYYMMDDstart, false);

	function YYYYDD(str) //年发生变化时日期发生变化(主要是判断闰平年)
	{
		var MMvalue = document.timesel.MM.options[document.timesel.MM.selectedIndex].value;
		if (MMvalue == "") {
			var e = document.timesel.DD;
			optionsClear(e);
			return;
		}
		var n = MonHead[MMvalue - 1];
		if (MMvalue == 2 && IsPinYear(str)) n++;
		writeDay(n)
	}

	function MMDD(str)   //月发生变化时日期联动
	{
		var YYYYvalue = document.timesel.YYYY.options[document.timesel.YYYY.selectedIndex].value;
		if (YYYYvalue == "") {
			var e = document.timesel.DD;
			optionsClear(e);
			return;
		}
		var n = MonHead[str - 1];
		if (str == 2 && IsPinYear(YYYYvalue)) n++;
		writeDay(n)
	}

	function writeDay(n)   //据条件写日期的下拉框
	{
		var e = document.timesel.DD;
		optionsClear(e);
		for (var i = 1; i < (n + 1); i++)
			e.options.add(new Option(" " + i + " 日", i));
	}

	function IsPinYear(year)//判断是否闰平年
	{
		return (0 == year % 4 && (year % 100 != 0 || year % 400 == 0));
	}

	function optionsClear(e) {
		e.options.length = 1;
	}

	function selall()
	{
		window.location.href="<%=request.getContextPath()%>/TestServlet/queryall";
	}

</script>

</head>

<body>
	<div class="wapper_box">
		<div class="wapper">
			<h2>温湿度查询系统</h2>
		</div>
		<div class="wapper">
			<div class="wapper_cell left">
				<div class="wapper_form">
					<form name="timesel" action="<%=request.getContextPath()%>/TestServlet/queryBydate" method="post">
						<table>
							<tr>
								<td colspan="2">
									<select name="YYYY" onchange="YYYYDD(this.value)">
									<option value="">请选择 年</option>
								</select>
									<select name="MM" onchange="MMDD(this.value)">
										<option value="">选择 月</option>
									</select>
									<select name="DD">
										<option value="">选择 日</option>
									</select>
								</td>
							</tr>
							<tr>
								<td><input type="button" value="查询所有" onclick="selall()"/></td>
								<td><input type="submit" value="查询"/></td>
							</tr>
						</table>
					</form>
				</div>
				<div class="wapper_table">
					<div class="wapper_title">
						<h3 class="wapper_time">时间</h3>
						<h3 class="wapper_cell1">温度</h3>
						<h3 class="wapper_cell2">湿度</h3>
					</div>
					<div class="wapper_info">
					
						<div class="wapper_time">
						<c:forEach items="${datalist}" var="data"><i>${data.time}</i><br/></c:forEach>
						</div>
						<div class="wapper_cell1">
						<c:forEach items="${datalist}" var="data"><i>${data.temperature}℃</i><br/></c:forEach>
						</div>
						<div class="wapper_cell2">
						<c:forEach items="${datalist}" var="data"><i>${data.humidity}%RH</i><br/></c:forEach>
						</div>
						
					</div>
				</div>
			</div>
			<div class="wapper_cell right">
				<div class="wapper_canvas">
					<div id="wendu" style="height:250px;width: 650px;"></div>
				</div>
				<div class="wapper_canvas">
					<div id="shidu" style="height:250px;width: 650px;"></div>
				</div>
				<script type="text/javascript">
					(function(){
						$.get("/Tcpsever/zhexian",function(data){
							var json=eval(data);
							let option = {
									title:{
										text:'温度'
									},
									tooltip:{
										tirgger:'axis'
									}, 
									xAxis: {
										name:'时间',
										type: 'category',
										data: json.map(function(item){
												return item.time.substring(11,item.time.length-2)
											})
									},
									yAxis: {
										name:'温度℃',
										type: 'value'
									},
									dataZoom: [{
							            startValue:json[0].time.substring(11,json[0].time.length-2)
							        }, {
							            type: 'inside'
							        }],
									series: [{
										data: json.map(function(item){
												return parseInt(item.temperature)
											}),
										type: 'line'
									}]
								};
								let myChart1=echarts.init(document.querySelector('#wendu'));
								let myChart2=echarts.init(document.querySelector('#shidu'));
								myChart1.setOption(option);
								myChart2.setOption(option);
								myChart2.setOption({
									title:{
										text:'湿度'
									},yAxis: {
										name:'湿度PH'
									},
									series: [{
										data: json.map(function(item){
												return parseInt(item.humidity)
											}),
										type: 'line'
									}]
								});
							
							});
						
						
					})();
				</script>
			</div>
		</div>
	</div>
</body>
</html>
