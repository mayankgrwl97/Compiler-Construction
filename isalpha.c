else if(isalpha(buff[i]))
	{
		int lo = 0;
		val[lo++] = buff[i];
		if(buff[i+1] == '\0')
		{
			buff = getStream(&cleanFile);
			if(buff[0] == '\0')
			{
				i = 0;
				val[lo] = '\0';
				if(presentsymboltable(table, val))
				{
					return makeToken(val, val, linenumber);
				}
				if(lo > 8)
				{
					return makeToken("ERROR_1", val, linenumber);
				}
				insertsymboltable(table, "ID", val, linenumber);
				return makeToken("ID", val, linenumber);
			}
			i = -1;
		}
		while(buff[i+1] == '_' || isdigit(buff[i+1]) || isalpha(buff[i+1]) )
		{
			i++;
			val[lo++] = buff[i];
			if(buff[i] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
				{
					i = 0;
					val[lo] = '\0';
					if(presentsymboltable(table, val))
					{
						return makeToken(val, val, linenumber);
					}
					if(lo > 8)
					{
						return makeToken("ERROR_1", val, linenumber);
					}
					insertsymboltable(table, "ID", val, linenumber);
					return makeToken("ID", val, linenumber);
				}
				i = -1;
			}
		}
		i++;
		val[lo] = '\0';
		if(presentsymboltable(table, val))
		{
			return makeToken(val, val, linenumber);
		}
		if(lo > 8)
		{
			return makeToken("ERROR_1", val, linenumber);
		}
		insertsymboltable(table, "ID", val, linenumber);
		return makeToken("ID", val, linenumber);
	}