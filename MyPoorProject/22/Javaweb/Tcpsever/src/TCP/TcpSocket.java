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
			ServerSocket ss = new ServerSocket(8883);// ����������
			Socket socket = ss.accept();// �ȴ�,�Է�Ӧ��󽫽���ͨ��,socket��¼��Ӧ����ܵ��ĵ�ַ�����ݣ�����ͨ���䷽�����ó���
			System.out.println("������"+socket);
			BufferedReader br=new BufferedReader(new InputStreamReader(socket.getInputStream()));
		//	BufferedWriter bw=new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
			while (true) {//������ȫ���԰�while������if���������������br�ж�����
				
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
					}//����json����
					try {
						dao.addData(datalist);//����dao���뵽���ݿ���
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
