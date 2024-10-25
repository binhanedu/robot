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
        //�������һ���ǹ̶�
        response.setContentType("text/html;charset=utf-8");          
        // ������Ӧͷ����ajax�������  
        response.setHeader("Access-Control-Allow-Origin", "*");  
        // �Ǻű�ʾ���е��������󶼿��Խ���  
        response.setHeader("Access-Control-Allow-Methods", "GET,POST");  
        
        //��ȡ΢��С����get�Ĳ���ֵ����ӡ
       /* String message = request.getParameter("message");
     
        System.out.println("message="+message);*/
        List<datamodel> datalist= new ArrayList<datamodel>();
		List<datamodel> newlist= new ArrayList<datamodel>();
		DataDao dao= new DataDao();
		try {
			
			datalist=dao.Query();
			if(datalist.size()>=10) {
				for(int i=datalist.size()-10;i<datalist.size();i++) {
					newlist.add(datalist.get(i));//ȡ���µ�10������
				}
		JSONArray json = JSONArray.fromObject(newlist);//��listת��ΪString����json��ʽ����������ȡ������ʾ
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
