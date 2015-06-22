package Tools;

import java.util.Vector;

public class UniqueUserName{
	public static String generateWord(Vector<String> userDB,String name , int id) {
		String builder = name+ "_" + id;
		if(userDB.contains(builder)) {
			return generateWord(userDB,builder,id);
		} else 
			return builder;
	}
}
