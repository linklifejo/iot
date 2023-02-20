package kr.icia.controller;

import static org.junit.Assert.fail;

import java.sql.Connection;
import java.sql.DriverManager;

import org.junit.Test;
import org.springframework.beans.factory.annotation.Autowired;

import kr.icia.mapper.LedMapper;
import lombok.Setter;
import lombok.extern.log4j.Log4j;

@Log4j
public class JDBCTests {

	@Setter(onMethod_ = @Autowired)
	private LedMapper mapper;

	static {
		try {
			Class.forName("oracle.jdbc.driver.OracleDriver");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
	}

	@Test
	public void testConnection() {
		try {
			Connection con = DriverManager.getConnection("jdbc:oracle:thin:@localhost:1521:XE", "hanul", "0000");
//			log.info(con);
			System.out.println(con);
		} catch (Exception e) {
			fail(e.getMessage());
		}

	}

	@Test
	public void testService() {
		System.out.println(mapper);
	}
}
