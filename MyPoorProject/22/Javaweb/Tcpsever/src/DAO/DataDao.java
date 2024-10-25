package DAO;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import DB.DBUtil;

import MODEL.datamodel;

public class DataDao {
	
	Connection conn=DBUtil.getConnection();
	
	public void addData(List<datamodel> list) throws Exception {//添加数据
		if (list.size()>0) {
			String SQL=""+"insert into iot(temperature,humidity,time)"
					+" values(?,?,?)";
			for (datamodel data : list) {
				
				Date date = new Date();//获得系统时间.
				SimpleDateFormat sdf =   new SimpleDateFormat( " yyyy-M-d  " );
				String nowTime = sdf.format(date);
				//执行sql语句
				PreparedStatement ptmt=conn.prepareStatement(SQL);
				ptmt.setString(1, data.getTemperature());
				ptmt.setString(2, data.getHumidity());
				ptmt.setString(3, nowTime);
				ptmt.execute();
			}
			
		} 
	
	}
	
	
	//取出所有历史数据并返回一个list
	 public List<datamodel> Query() throws Exception{	                                 
		List<datamodel> datalist = new ArrayList<datamodel>();
		String sql = "select * from iot";
		PreparedStatement pstmt = conn.prepareStatement(sql);
		ResultSet result = pstmt.executeQuery();
		while(result.next()) {
			datamodel data = new datamodel();
			data.setTemperature(result.getString("temperature"));
			data.setHumidity(result.getString("humidity"));
			data.setTime(result.getString("time"));
			datalist.add(data);
		}
		return datalist;
	}
	
	 
	//根据年月日进行模糊查询并返回一个list
	 public List<datamodel> querybydate(String date) throws Exception{
			List<datamodel> datalist=new ArrayList<datamodel>();
			StringBuilder sb=new StringBuilder();
			sb.append("select * from iot ");
			sb.append(" where time like ? ");
			PreparedStatement ptmt=conn.prepareStatement(sb.toString());
			ptmt.setString(1, "%"+date+"%");
			ResultSet rs=ptmt.executeQuery();
			datamodel g=null;
			while(rs.next()){
				g=new datamodel();
				g.setTemperature(rs.getString("temperature"));
				g.setHumidity(rs.getString("humidity"));
				g.setTime(rs.getString("time"));
				datalist.add(g);
			}
			return datalist;
		}
}
