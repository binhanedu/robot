package dataServlet;

import java.io.IOException;
import java.io.PrintWriter;
import java.text.DecimalFormat;
import java.util.*;


import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import DAO.DataDao;
import MODEL.datamodel;
import net.sf.json.JSONArray;

@WebServlet("/TestServlet/*")
public class TestServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		doPost(request, response);
		
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String path = request.getPathInfo();
		
		if (path.equals("/*")){
			System.out.println(path);
		} else if (path.equals("/queryall")) {
			queryall(request,response);
			System.out.println(path);
		System.out.println(1);
		}else if(path.equals("/queryBydate")) {
			queryBydate(request,response);
			System.out.println(path);
		}
		
	}
	
	
	
			
	private void queryall(HttpServletRequest request, HttpServletResponse response)	{
		//处理查询所有数据
	List<datamodel> datalist= new ArrayList<datamodel>();
	DataDao dao= new DataDao();	
	try {
		datalist=dao.Query();
	} catch (Exception e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
		if(!datalist.isEmpty()) {
				request.getSession().setAttribute("datalist", datalist);
				try {
					response.sendRedirect("/Tcpsever/jsp/data1.jsp");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}else {
				try {
					response.sendRedirect("/Tcpsever/jsp/error.jsp");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		
}		
	
	
	
	
	private void queryBydate(HttpServletRequest request, HttpServletResponse response) {
		//处理单日期查询
		List<datamodel> datalist= new ArrayList<datamodel>();
		DataDao dao= new DataDao();
			String year=request.getParameter("YYYY");
			String month=request.getParameter("MM");
			DecimalFormat df=new DecimalFormat("00");
			String month2=df.format(Integer.parseInt(month));
			String day=request.getParameter("DD");
			String day2=df.format(Integer.parseInt(day));
			String str = year+"-"+month2+"-"+day2;
			try {
				datalist=dao.querybydate(str);
				//System.out.println(datalist);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(!datalist.isEmpty()) {
				request.getSession().setAttribute("datalist", datalist);
				try {
					response.sendRedirect("/Tcpsever/jsp/data1.jsp");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}else {
				try {
					response.sendRedirect("/Tcpsever/jsp/error.jsp");
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
	}
	
	

}
