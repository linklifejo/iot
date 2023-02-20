package kr.icia.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import kr.icia.mapper.LedMapper;
import lombok.Setter;

@Service
public class LedServiceImp implements LedService {
	@Setter(onMethod_ = @Autowired)
	private LedMapper mapper;

	@Override
	public void register(String onOff) {
		mapper.insert(onOff);
	}
}
