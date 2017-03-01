if(isdigit(buff[i]))
	{
		int lo = 0; //pinter for val char array
		val[lo++] = buff[i]; //inserting first digit scanned

		if(buff[i+1] == '\0')
		{
			buff = getStream(&cleanFile);
			if(buff[0] == '\0')
			{
				i = 0;
				val[lo] = '\0';
				return makeToken("NUM", val, linenumber);
			}
			i = -1;
		}
		while(isdigit(buff[i+1]))
		{
			i++;
			val[lo++] = buff[i];
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
				{
					i = 0;
					val[lo] = '\0';
					return makeToken("NUM", val, linenumber);
				}
				i = -1;
			}
		}
		if(buff[i+1] == '.')
		{
			i++;
			val[lo++] = buff[i];
			if(buff[i+1] == '\0')
			{
				buff = getStream(&cleanFile);
				if(buff[0] == '\0')
				{
					i=0;
					val[lo] = '\0';
					return makeToken("ERROR_3", val, linenumber);
				}
				i = -1;
			}
			if(buff[i+1] == '.')
			{
				// currently on the first .
				// no need for i--
				val[lo-1] = '\0';
				return makeToken("NUM", val, linenumber);
			}
			else if(isdigit(buff[i+1]))
			{
				i++;
				val[lo++] = buff[i];
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i = 0;
						val[lo] = '\0';
						return makeToken("RNUM", val, linenumber);
					}
					i = -1;
				}
				while(isdigit(buff[i+1]))
				{
					i++;
					val[lo++] = buff[i];
					if(buff[i+1] == '\0')
					{
						buff = getStream(&cleanFile);
						if(buff[0] == '\0')
						{
							i = 0;
							val[lo] = '\0';
							return makeToken("RNUM", val, linenumber);
						}
						i = -1;
					}
				}
				if(buff[i+1] == 'e' || buff[i+1] == 'E')
				{
					i++;
					val[lo++] = buff[i];
					if(buff[i+1] == '\0')
					{
						buff = getStream(&cleanFile);
						if(buff[0] == '\0')
						{
							i = 0;
							val[lo] = '\0';
							return makeToken("ERROR_3", val, linenumber);
						}
						i = -1;
					}
					if(buff[i+1] == '+' || buff[i+1] == '-')
					{
						i++;
						val[lo++] = buff[i];
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i = 0;
								val[lo] = '\0';
								return makeToken("ERROR_3", val, linenumber);
							}
							i = -1;
						}
						if(isdigit(buff[i+1]))
						{
							i++;
							val[lo++] = buff[i];
							if(buff[i+1] == '\0')
							{
								buff = getStream(&cleanFile);
								if(buff[0] == '\0')
								{
									i = 0;
									val[lo] = '\0';
									return makeToken("RNUM", val, linenumber);
								}
								i = -1;
							}
							while(isdigit(buff[i+1]))
							{
								i++;
								val[lo++] = buff[i];
								if(buff[i+1] == '\0')
								{
									buff = getStream(&cleanFile);
									if(buff[0] == '\0')
									{
										i = 0;
										val[lo] = '\0';
										return makeToken("RNUM",val, linenumber);
									}
									i = -1;
								}
							}
							if(isdelim(buff[i+1]))
							{
								i++;
								val[lo] = '\0';
								return makeToken("RNUM",val, linenumber);
							}
							while(!isdelim(buff[i+1]))
							{
								i++;
								val[lo++] = buff[i];
								if(buff[i+1] == '\0')
								{
									buff = getStream(&cleanFile);
									if(buff[0] == '\0')
									{
										i = 0;
										val[lo] = '\0';
										return makeToken("ERROR_3", val, linenumber);
									}
									i = -1;
								}
							}
							i++;
							val[lo] = '\0';
							return makeToken("ERROR_3", val, linenumber);
						}
						else
						{
							while(!isdelim(buff[i+1]))
							{
								i++;
								val[lo++] = buff[i];
								if(buff[i+1] == '\0')
								{
									buff = getStream(&cleanFile);
									if(buff[0] == '\0')
									{
										i = 0;
										val[lo] = '\0';
										return makeToken("ERROR_3", val, linenumber);
									}
									i = -1;
								}
							}
							i++;
							val[lo] = '\0';
							return makeToken("ERROR_3", val, linenumber);
						}
					}
					else if(isdigit(buff[i+1]))
					{
						i++;
						val[lo++] = buff[i];
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i = 0;
								val[lo] = '\0';
								return makeToken("RNUM", val, linenumber);
							}
							i = -1;
						}
						while(isdigit(buff[i+1]))
						{
							i++;
							val[lo++] = buff[i];
							if(buff[i+1] == '\0')
							{
								buff = getStream(&cleanFile);
								if(buff[0] == '\0')
								{
									i = 0;
									val[lo] = '\0';
									return makeToken("RNUM", val, linenumber);
								}
								i = -1;
							}
						}
						if(isdelim(buff[i+1]))
						{
							i++;
							val[lo] = '\0';
							return makeToken("RNUM", val, linenumber);
						}
						while(!isdelim(buff[i+1]))
						{
							i++;
							val[lo++] = buff[i];
							if(buff[i+1] == '\0')
							{
								buff = getStream(&cleanFile);
								if(buff[0] == '\0')
								{
									i = 0;
									val[lo] = '\0';
									return makeToken("ERROR_3", val, linenumber);
								}
								i = -1;
							}
						}
						i++;
						val[lo] = '\0';
						return makeToken("ERROR_3", val, linenumber);
					}
					else
					{
						while(!isdelim(buff[i+1]))
						{
							i++;
							val[lo++] = buff[i];
							if(buff[i+1] == '\0')
							{
								buff = getStream(&cleanFile);
								if(buff[0] == '\0')
								{
									i = 0;
									val[lo] = '\0';
									return makeToken("ERROR_3", val, linenumber);
								}
								i = -1;
							}
						}
						i++;
						val[lo] = '\0';
						return makeToken("ERROR_3", val, linenumber);
					}
				}
				else
				{
					if(isdelim(buff[i+1]))
					{
						i++;
						val[lo] = '\0';
						return makeToken("RNUM", val, linenumber);
					}
					while(!isdelim(buff[i+1]))
					{
						i++;
						val[lo++] = buff[i];
						if(buff[i+1] == '\0')
						{
							buff = getStream(&cleanFile);
							if(buff[0] == '\0')
							{
								i = 0;
								val[lo] = '\0';
								return makeToken("ERROR_3", val, linenumber);
							}
							i = -1;
						}
					}
					i++;
					val[lo] = '\0';
					return makeToken("ERROR_3", val, linenumber);
				}
			}
			else
			{
				while(!isdelim(buff[i+1]))
				{
					i++;
					val[lo++] = buff[i];
					if(buff[i+1] == '\0')
					{
						buff = getStream(&cleanFile);
						if(buff[0] == '\0')
						{
							i = 0;
							val[lo] = '\0';
							return makeToken("ERROR_3", val, linenumber);
						}
						i = -1;
					}
				}
				i++;
				val[lo] = '\0';
				return makeToken("ERROR_3", val, linenumber);
			}			
		}
		else
		{
			if(isdelim(buff[i+1]))
			{
				i++;
				val[lo] = '\0';
				return makeToken("NUM", val, linenumber);
			}
			while(!isdelim(buff[i+1]))
			{
				i++;
				val[lo++] = buff[i];
				if(buff[i+1] == '\0')
				{
					buff = getStream(&cleanFile);
					if(buff[0] == '\0')
					{
						i = 0;
						val[lo] = '\0';
						return makeToken("ERROR_3", val, linenumber);
					}
					i = -1;
				}
			}
			i++;
			val[lo] = '\0';
			return makeToken("ERROR_3", val, linenumber);
		}
	}