package dataServlet;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import DAO.DataDao;
import MODEL.datamodel;
import net.sf.json.JSONArray;


@WebServlet("/zhexian")
public class zhexianServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
   
   
	
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
	
	//	
		List<datamodel> datalist= new ArrayList<datamodel>();
		List<datamodel> newlist= new ArrayList<datamodel>();
		DataDao dao= new DataDao();
		try {
			
			datalist=dao.Query();
			if(datalist.size()>=10) {
				for(int i=datalist.size()-10;i<datalist.size();i++) {
					newlist.add(datalist.get(i));//取最新的10条数据
				}
		JSONArray json = JSONArray.fromObject(newlist);//将list转化为json格式，便于折线取数据显示
		String str=json.toString();
		PrintWriter out=response.getWriter();
		out.println(str);
		//response.sendRedirect("/Tcpsever/jsp/data1.jsp");
		System.out.println(str);
		
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}
	

	
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		
		doGet(request, response);
		
}
	
}
