#include "ErrorCode.h"
#include <stdio.h>
#include<iostream>
class ErrorCode {
	public:
		int errCode;
		status isIgnore;
		
		ErrorCode(int errCode ,status isIgnore=Ignore):errCode(errCode),isIgnore(isIgnore){}
		ErrorCode(){
			errCode=0;
			isIgnore=Ignore;
		}
		void addErrorCode(ErrorID err){
			errCode=+pow(2,(int)err);
		}
};
