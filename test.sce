obj print(obj s){
	return __write__(stdout, s);
}
obj println(obj s){
	return __write__(stdout, s + "\n");
}
obj scan(){
	return __read__(stdin);
}
obj add(obj a, obj b){
	return a + b;
}
name EVENT1{
	start{
		obj test = 0;
		obj i = {test,6,2,3,1,7};
		obj idx = 0;
		obj sum = 0;
		while(idx < i.size()){
			sum += i[idx];
			idx += 1;
		}
		obj ave = sum / i.size();
		print("DATA SUM ");
		println(sum);
		print("DATA AVE ");
		println(ave);
	}
}