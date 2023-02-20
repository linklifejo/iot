package kr.icia.controller;

import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;

import kr.icia.service.LedService;
import lombok.AllArgsConstructor;

@Controller
@AllArgsConstructor
public class LedController {
	private LedService service;

	@GetMapping("/node")
	public ResponseEntity<String> node(@RequestParam("led") String s) {
		service.register(s);

		return new ResponseEntity<String>("ok", HttpStatus.OK);
	}

}
