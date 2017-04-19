TIGER_PARAMETERS(nullptr); //Add parameter names here. These are accessed from data.param<P("name")>();
TIGER_CHANNELS("red","green","blue"); //Or suitable names
TIGER_ENTRY(process); //Define the name of the entry point

static void process(const Tiger::FilterStateClient& data,unsigned long long int frame)
	{
	auto w=data.width();
	auto h=data.height();
	for(int y=0;y<h;++y)
		{
		for(int x=0;x<w;++x)
			{
			data.value_next<C("red")>(x,y)=data.value_current<C("red")>(x,y);
			data.value_next<C("green")>(x,y)=data.value_current<C("green")>(x,y);
			data.value_next<C("blue")>(x,y)=data.value_current<C("blue")>(x,y);
			}
		}
	}
