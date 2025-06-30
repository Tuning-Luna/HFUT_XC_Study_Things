public class test {
	public static void main(String[] args) {
		String[][] a = new String[3][4];
		a[2][0] = String.valueOf(9); // 将 int 转换为 String
		a[2][1] = String.valueOf(10); // 将 int 转换为 String
		System.out.printf(a[2][0], a[2][1]); // 修改此处，使用 a[2][0] 作为格式字符串
	}
}