package JSON;


import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;


import MODEL.datamodel;
import net.sf.json.*;

public class JsonUtils {//json字符串转list
	public List<datamodel> getJsonList(char[] str)  throws Exception{
		String str1 = new String(str);//将char[]转化为字符串
		 //System.out.println(str1.toString());
		 
        JSONObject json1 = JSONObject.fromObject(str1);
       // System.out.println(json1.toString());
       
		datamodel user = new datamodel();
		user.setTemperature(json1.getString("temperature"));
		user.setHumidity(json1.getString("humidity"));
		List<datamodel> datalist = new ArrayList<datamodel>();
		datalist.add(user);
		return datalist;
}
}
