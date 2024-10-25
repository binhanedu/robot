package dataServlet;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.Writer;
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

@WebServlet("/loginTestServlet")
public class loginTestServlet extends HttpServlet {
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        //这里代码一般是固定
        response.setContentType("text/html;charset=utf-8");          
        // 设置响应头允许ajax跨域访问  
        response.setHeader("Access-Control-Allow-Origin", "*");  
        // 星号表示所有的异域请求都可以接受  
        response.setHeader("Access-Control-Allow-Methods", "GET,POST");  
        
        //获取微信小程序get的参数值并打印
       /* String message = request.getParameter("message");
     
        System.out.println("message="+message);*/
        List<datamodel> datalist= new ArrayList<datamodel>();
		List<datamodel> newlist= new ArrayList<datamodel>();
		DataDao dao= new DataDao();
		try {
			
			datalist=dao.Query();
			if(datalist.size()>=10) {
				for(int i=datalist.size()-10;i<datalist.size();i++) {
					newlist.add(datalist.get(i));//取最新的10条数据
				}
		JSONArray json = JSONArray.fromObject(newlist);//将list转化为String类型json格式，便于折线取数据显示
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

    public void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {

    }

}
