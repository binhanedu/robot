package TCP;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import DAO.DataDao;
import JSON.JsonUtils;
import MODEL.datamodel;
public class TcpSocket {
 
	public static void main(String[] args) {
		try {
			ServerSocket ss = new ServerSocket(8883);// 创建服务器
			Socket socket = ss.accept();// 等待,对方应答后将建立通道,socket记录了应答接受到的地址和内容，可以通过其方法调用出来
			System.out.println("服务器"+socket);
			BufferedReader br=new BufferedReader(new InputStreamReader(socket.getInputStream()));
		//	BufferedWriter bw=new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
			while (true) {//这里完全可以把while跟下面if结合起来，条件是br有东西读
				
				char[]str = new char[1024];
			
				if((br.read(str))!=-1)
				{	
					if(str.equals(" "))
					{
						System.out.println("error");
						break;
					}
					System.out.println(str);
					DataDao dao=new DataDao();
					JsonUtils json=new JsonUtils();
					List<datamodel> datalist = new ArrayList<datamodel>();
					try {
						datalist=json.getJsonList(str);
					} catch (Exception e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}//解析json数据
					try {
						dao.addData(datalist);//调用dao加入到数据库中
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
			br.close();
			socket.close();
			ss.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
