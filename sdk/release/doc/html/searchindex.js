Search.setIndex({envversion:46,filenames:["alchemy","blackscholes","broker","broker_interact","data_sources","data_struct","firstcode","generators","history","index","install","ml","mlstruct","news","quant","record","records","rss","samples","start","ta","yahoo"],objects:{"":{"quotek::broker":[2,0,1,"_CPPv2N6quotek6brokerE"],"quotek::broker::broker":[2,1,1,"_CPPv2N6quotek6broker6brokerERN6quotek4core8strategyE"],"quotek::broker::buy":[2,1,1,"_CPPv2N6quotek6broker3buyENSt6stringEiii"],"quotek::broker::buy_hedged":[2,1,1,"_CPPv2N6quotek6broker10buy_hedgedENSt6stringENSt6stringEiii"],"quotek::broker::buy_hedged_asym":[2,1,1,"_CPPv2N6quotek6broker15buy_hedged_asymENSt6stringEiiiNSt6stringEiii"],"quotek::broker::close_position":[2,1,1,"_CPPv2N6quotek6broker14close_positionENSt6stringE"],"quotek::broker::sell":[2,1,1,"_CPPv2N6quotek6broker4sellENSt6stringEiii"],"quotek::broker::smartbuy":[2,1,1,"_CPPv2N6quotek6broker8smartbuyENSt6stringEi"],"quotek::broker::smartsell":[2,1,1,"_CPPv2N6quotek6broker9smartsellENSt6stringEi"],"quotek::data":[5,3,1,"_CPPv2N6quotek4dataE"],"quotek::data::any":[5,0,1,"_CPPv2N6quotek4data3anyE"],"quotek::data::cqueue":[5,0,1,"_CPPv2N6quotek4data6cqueueE"],"quotek::data::cvector":[5,0,1,"_CPPv2N6quotek4data7cvectorE"],"quotek::data::history":[5,0,1,"_CPPv2N6quotek4data7historyE"],"quotek::data::history::close":[8,2,1,"_CPPv2N6quotek4data7history5closeE"],"quotek::data::history::high":[8,2,1,"_CPPv2N6quotek4data7history4highE"],"quotek::data::history::history":[8,1,1,"_CPPv2N6quotek4data7history7historyEv"],"quotek::data::history::low":[8,2,1,"_CPPv2N6quotek4data7history3lowE"],"quotek::data::history::open":[8,2,1,"_CPPv2N6quotek4data7history4openE"],"quotek::data::history::timestamp":[8,2,1,"_CPPv2N6quotek4data7history9timestampE"],"quotek::data::history::~history":[8,1,1,"_CPPv2N6quotek4data7historyD0Ev"],"quotek::data::news":[5,0,1,"_CPPv2N6quotek4data4newsE"],"quotek::data::news::content":[13,2,1,"_CPPv2N6quotek4data4news7contentE"],"quotek::data::news::date":[13,2,1,"_CPPv2N6quotek4data4news4dateE"],"quotek::data::news::news":[13,1,1,"_CPPv2N6quotek4data4news4newsEv"],"quotek::data::news::source":[13,2,1,"_CPPv2N6quotek4data4news6sourceE"],"quotek::data::news::tags":[13,2,1,"_CPPv2N6quotek4data4news4tagsE"],"quotek::data::news::title":[13,2,1,"_CPPv2N6quotek4data4news5titleE"],"quotek::data::news::~news":[13,1,1,"_CPPv2N6quotek4data4newsD0Ev"],"quotek::data::record":[5,0,1,"_CPPv2N6quotek4data6recordE"],"quotek::data::record::extract":[15,1,1,"_CPPv2N6quotek4data6record7extractERNSt6vectorI6recordEEii"],"quotek::data::record::max":[15,1,1,"_CPPv2N6quotek4data6record3maxERNSt6vectorI6recordEE"],"quotek::data::record::min":[15,1,1,"_CPPv2N6quotek4data6record3minERNSt6vectorI6recordEE"],"quotek::data::record::record":[15,1,1,"_CPPv2N6quotek4data6record6recordEv"],"quotek::data::record::sample":[15,1,1,"_CPPv2N6quotek4data6record6sampleERNSt6vectorI6recordEEll"],"quotek::data::record::search":[15,1,1,"_CPPv2N6quotek4data6record6searchERNSt6vectorI6recordEEl"],"quotek::data::record::spread":[15,2,1,"_CPPv2N6quotek4data6record6spreadE"],"quotek::data::record::time_as_value":[15,1,1,"_CPPv2N6quotek4data6record13time_as_valueERNSt6vectorI6recordEE"],"quotek::data::record::timestamp":[15,2,1,"_CPPv2N6quotek4data6record9timestampE"],"quotek::data::record::timestamps_export":[15,1,1,"_CPPv2N6quotek4data6record17timestamps_exportERNSt6vectorI6recordEE"],"quotek::data::record::value":[15,2,1,"_CPPv2N6quotek4data6record5valueE"],"quotek::data::record::values_export":[15,1,1,"_CPPv2N6quotek4data6record13values_exportERNSt6vectorI6recordEE"],"quotek::data::record::values_import":[15,1,1,"_CPPv2N6quotek4data6record13values_importERNSt6vectorIfEE"],"quotek::data::record::~record":[15,1,1,"_CPPv2N6quotek4data6recordD0Ev"],"quotek::data::records":[5,0,1,"_CPPv2N6quotek4data7recordsE"],"quotek::data::records::append":[16,1,1,"_CPPv2N6quotek4data7records6appendElf"],"quotek::data::records::down_sample":[16,1,1,"_CPPv2N6quotek4data7records11down_sampleEifNSt6stringE"],"quotek::data::records::get_data":[16,1,1,"_CPPv2N6quotek4data7records8get_dataEv"],"quotek::data::records::get_spreads":[16,1,1,"_CPPv2N6quotek4data7records11get_spreadsEv"],"quotek::data::records::get_timestamps":[16,1,1,"_CPPv2N6quotek4data7records14get_timestampsEv"],"quotek::data::records::get_values":[16,1,1,"_CPPv2N6quotek4data7records10get_valuesEv"],"quotek::data::records::last":[16,1,1,"_CPPv2N6quotek4data7records4lastEv"],"quotek::data::records::operator[]":[16,1,1,"_CPPv2N6quotek4data7recordsixERKi"],"quotek::data::records::records":[16,1,1,"_CPPv2N6quotek4data7records7recordsEv"],"quotek::data::records::size":[16,1,1,"_CPPv2N6quotek4data7records4sizeEv"],"quotek::data::records::~records":[16,1,1,"_CPPv2N6quotek4data7recordsD0Ev"],"quotek::datasource":[4,3,1,"_CPPv2N6quotek10datasourceE"],"quotek::datasource::datasource":[4,0,1,"_CPPv2N6quotek10datasource10datasourceE"],"quotek::datasource::rss":[4,0,1,"_CPPv2N6quotek10datasource3rssE"],"quotek::datasource::rss::fetch":[17,1,1,"_CPPv2N6quotek10datasource3rss5fetchEi"],"quotek::datasource::rss::rss":[17,1,1,"_CPPv2N6quotek10datasource3rss3rssENSt6stringE"],"quotek::datasource::rss::~rss":[17,1,1,"_CPPv2N6quotek10datasource3rssD0Ev"],"quotek::datasource::yahoofinance":[4,0,1,"_CPPv2N6quotek10datasource12yahoofinanceE"],"quotek::datasource::yahoofinance::close_history":[21,1,1,"_CPPv2N6quotek10datasource12yahoofinance13close_historyENSt6stringENSt6stringENSt6stringE"],"quotek::datasource::yahoofinance::history":[21,1,1,"_CPPv2N6quotek10datasource12yahoofinance7historyENSt6stringENSt6stringENSt6stringE"],"quotek::datasource::yahoofinance::yahoofinance":[21,1,1,"_CPPv2N6quotek10datasource12yahoofinance12yahoofinanceEv"],"quotek::datasource::yahoofinance::~yahoofinance":[21,1,1,"_CPPv2N6quotek10datasource12yahoofinanceD0Ev"],"quotek::ml::action":[12,0,1,"_CPPv2N6quotek2ml6actionE"],"quotek::ml::action::negated":[12,2,1,"_CPPv2N6quotek2ml6action7negatedE"],"quotek::ml::action::tense":[12,2,1,"_CPPv2N6quotek2ml6action5tenseE"],"quotek::ml::action::text":[12,2,1,"_CPPv2N6quotek2ml6action4textE"],"quotek::ml::alchemy":[0,0,1,"_CPPv2N6quotek2ml7alchemyE"],"quotek::ml::alchemy::alchemy":[0,1,1,"_CPPv2N6quotek2ml7alchemy7alchemyENSt6stringENSt6stringE"],"quotek::ml::alchemy::language":[0,1,1,"_CPPv2N6quotek2ml7alchemy8languageENSt6stringENSt6stringE"],"quotek::ml::alchemy::raw":[0,1,1,"_CPPv2N6quotek2ml7alchemy3rawENSt6stringENSt3mapINSt6stringENSt6stringEEE"],"quotek::ml::alchemy::relations":[0,1,1,"_CPPv2N6quotek2ml7alchemy9relationsENSt6stringENSt6stringE"],"quotek::ml::alchemy::sentiment":[0,1,1,"_CPPv2N6quotek2ml7alchemy9sentimentENSt6stringENSt6stringENSt6stringE"],"quotek::ml::alchemy::~alchemy":[0,1,1,"_CPPv2N6quotek2ml7alchemyD0Ev"],"quotek::ml::relation":[12,0,1,"_CPPv2N6quotek2ml8relationE"],"quotek::ml::relation::action":[12,2,1,"_CPPv2N6quotek2ml8relation6actionE"],"quotek::ml::relation::object":[12,2,1,"_CPPv2N6quotek2ml8relation6objectE"],"quotek::ml::relation::sentence":[12,2,1,"_CPPv2N6quotek2ml8relation8sentenceE"],"quotek::ml::relation::subject":[12,2,1,"_CPPv2N6quotek2ml8relation7subjectE"],"quotek::ml::sentiment":[12,0,1,"_CPPv2N6quotek2ml9sentimentE"],"quotek::ml::sentiment::mixed":[12,2,1,"_CPPv2N6quotek2ml9sentiment5mixedE"],"quotek::ml::sentiment::positive":[12,2,1,"_CPPv2N6quotek2ml9sentiment8positiveE"],"quotek::ml::sentiment::score":[12,2,1,"_CPPv2N6quotek2ml9sentiment5scoreE"],"quotek::quant":[14,3,1,"_CPPv2N6quotek5quantE"],"quotek::quant::_greeks":[14,3,1,"_CPPv2N6quotek5quant7_greeksE"],"quotek::quant::_greeks::delta":[14,2,1,"_CPPv2N6quotek5quant7_greeks5deltaE"],"quotek::quant::_greeks::gamma":[14,2,1,"_CPPv2N6quotek5quant7_greeks5gammaE"],"quotek::quant::_greeks::rho":[14,2,1,"_CPPv2N6quotek5quant7_greeks3rhoE"],"quotek::quant::_greeks::theta":[14,2,1,"_CPPv2N6quotek5quant7_greeks5thetaE"],"quotek::quant::_greeks::vega":[14,2,1,"_CPPv2N6quotek5quant7_greeks4vegaE"],"quotek::quant::above":[14,1,1,"_CPPv2N6quotek5quant5aboveERNSt6vectorIN6quotek4data6recordEEEff"],"quotek::quant::affine":[14,3,1,"_CPPv2N6quotek5quant6affineE"],"quotek::quant::affine::a":[14,2,1,"_CPPv2N6quotek5quant6affine1aE"],"quotek::quant::affine::b":[14,2,1,"_CPPv2N6quotek5quant6affine1bE"],"quotek::quant::average":[14,1,1,"_CPPv2N6quotek5quant7averageERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::blackscholes":[14,0,1,"_CPPv2N6quotek5quant12blackscholesE"],"quotek::quant::blackscholes::blackscholes":[1,1,1,"_CPPv2N6quotek5quant12blackscholes12blackscholesER6optionfff"],"quotek::quant::blackscholes::compute_greeks":[1,1,1,"_CPPv2N6quotek5quant12blackscholes14compute_greeksEv"],"quotek::quant::blackscholes::~blackscholes":[1,1,1,"_CPPv2N6quotek5quant12blackscholesD0Ev"],"quotek::quant::covariance":[14,1,1,"_CPPv2N6quotek5quant10covarianceERNSt6vectorIN6quotek4data6recordEEERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::covariance_q":[14,1,1,"_CPPv2N6quotek5quant12covariance_qERNSt6vectorIN6quotek4data6recordEEERNSt6vectorIN6quotek4data6recordEEEff"],"quotek::quant::crosses":[14,1,1,"_CPPv2N6quotek5quant7crossesERNSt6vectorIN6quotek4data6recordEEERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::exponential_moving_average":[14,1,1,"_CPPv2N6quotek5quant26exponential_moving_averageERNSt6vectorIN6quotek4data6recordEEEi"],"quotek::quant::linear_regression":[14,1,1,"_CPPv2N6quotek5quant17linear_regressionERNSt6vectorIN6quotek4data6recordEEERNSt6vectorIfEE"],"quotek::quant::max":[14,1,1,"_CPPv2N6quotek5quant3maxERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::min":[14,1,1,"_CPPv2N6quotek5quant3minERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::moving_average":[14,1,1,"_CPPv2N6quotek5quant14moving_averageERNSt6vectorIN6quotek4data6recordEEEi"],"quotek::quant::normal_cumulative_distribution":[14,1,1,"_CPPv2N6quotek5quant30normal_cumulative_distributionEf"],"quotek::quant::option":[14,0,1,"_CPPv2N6quotek5quant6optionE"],"quotek::quant::option::expires":[1,2,1,"_CPPv2N6quotek5quant6option7expiresE"],"quotek::quant::option::is_in_money":[1,1,1,"_CPPv2N6quotek5quant6option11is_in_moneyEv"],"quotek::quant::option::option":[1,1,1,"_CPPv2N6quotek5quant6option6optionEv"],"quotek::quant::option::price":[1,2,1,"_CPPv2N6quotek5quant6option5priceE"],"quotek::quant::option::strike":[1,2,1,"_CPPv2N6quotek5quant6option6strikeE"],"quotek::quant::option::type":[1,2,1,"_CPPv2N6quotek5quant6option4typeE"],"quotek::quant::option::underlying":[1,2,1,"_CPPv2N6quotek5quant6option10underlyingE"],"quotek::quant::option::~option":[1,1,1,"_CPPv2N6quotek5quant6optionD0Ev"],"quotek::quant::percent_delta":[14,1,1,"_CPPv2N6quotek5quant13percent_deltaERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::polynomial_regression":[14,1,1,"_CPPv2N6quotek5quant21polynomial_regressionERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::risk_reward_ratio":[14,1,1,"_CPPv2N6quotek5quant17risk_reward_ratioEfii"],"quotek::quant::standard_deviation":[14,1,1,"_CPPv2N6quotek5quant18standard_deviationERNSt6vectorIN6quotek4data6recordEEEb"],"quotek::quant::standard_normal_distribution":[14,1,1,"_CPPv2N6quotek5quant28standard_normal_distributionEf"],"quotek::quant::trend_p":[14,3,1,"_CPPv2N6quotek5quant7trend_pE"],"quotek::quant::trend_p::bear":[14,2,1,"_CPPv2N6quotek5quant7trend_p4bearE"],"quotek::quant::trend_p::bull":[14,2,1,"_CPPv2N6quotek5quant7trend_p4bullE"],"quotek::quant::trend_p::neutral":[14,2,1,"_CPPv2N6quotek5quant7trend_p7neutralE"],"quotek::quant::trend_percentages":[14,1,1,"_CPPv2N6quotek5quant17trend_percentagesERNSt6vectorIN6quotek4data6recordEEE"],"quotek::quant::trinom":[14,3,1,"_CPPv2N6quotek5quant6trinomE"],"quotek::quant::variance":[14,1,1,"_CPPv2N6quotek5quant8varianceERNSt6vectorIN6quotek4data6recordEEEb"],"quotek::quant::variance_q":[14,1,1,"_CPPv2N6quotek5quant10variance_qERNSt6vectorIN6quotek4data6recordEEEbf"],"quotek::quant::weighted_average":[14,1,1,"_CPPv2N6quotek5quant16weighted_averageERNSt6vectorIN6quotek4data6recordEEERNSt6vectorIiEE"],"quotek::quant::weighted_moving_average":[14,1,1,"_CPPv2N6quotek5quant23weighted_moving_averageERNSt6vectorIN6quotek4data6recordEEEi"],"quotek::rand::generators":[7,3,1,"_CPPv2N6quotek4rand10generatorsE"],"quotek::rand::generators::binomial":[7,1,1,"_CPPv2N6quotek4rand10generators8binomialEiif"],"quotek::rand::generators::lognormal":[7,1,1,"_CPPv2N6quotek4rand10generators9lognormalEiff"],"quotek::rand::generators::normal":[7,1,1,"_CPPv2N6quotek4rand10generators6normalEiff"],"quotek::rand::generators::poisson":[7,1,1,"_CPPv2N6quotek4rand10generators7poissonEii"],"quotek::rand::generators::uniform":[7,1,1,"_CPPv2N6quotek4rand10generators7uniformEiff"],"quotek::ta":[20,3,1,"_CPPv2N6quotek2taE"],"quotek::ta::moving_average_convergeance_divergeance":[20,1,1,"_CPPv2N6quotek2ta39moving_average_convergeance_divergeanceERNSt6vectorIN6quotek4data6recordEEEiii"]}},objnames:{"0":["cpp","class","C++ class"],"1":["cpp","function","C++ function"],"2":["cpp","member","C++ member"],"3":["cpp","type","C++ type"]},objtypes:{"0":"cpp:class","1":"cpp:function","2":"cpp:member","3":"cpp:type"},terms:{"15minut":18,"2nd":14,"3_7031":18,"50sec":18,"50second":18,"_file":10,"_greek":[1,14],"abstract":7,"case":15,"char":6,"class":[0,2,4,8],"const":16,"default":[1,14],"export":15,"final":[6,10],"float":[1,6,7,8,12,14,15,16],"function":[0,1,2,5,6,7,8,9,13,14,15,16,17,20,21],"import":[5,9,15],"int":[1,2,6,7,14,15,16,17,18,20],"long":[2,8,13,15,16,20],"new":2,"public":[0,1,2,8,12,13,14,15,16,17,21],"return":[0,1,2,5,7,14,15,16,17,20,21],"short":[2,9,20],"static":15,"true":14,"try":18,"var":21,"void":[2,14,16],"while":[5,8],abl:[0,5],about:[0,5,8,9,14,16,18],abov:[14,20],access:18,accomplish:1,accord:2,accuraci:14,across:5,action:12,add:[0,7,10,14,16],addit:10,adjust:8,administr:10,affin:14,against:14,aggreg:[14,16],agrreg:16,aim:[0,5,8,13,14],alchemyapi:[0,18],algo:[0,9],algorithm:[9,12,18],all:[2,6,9,10,14,15,16,18],allow:[0,2,4,7,9,12,15,17,21],along:[2,5],alreadi:[10,14],also:[0,7,10,14,18,20],among:9,amount:[5,9,16],analys:0,analysi:0,analyz:5,ani:[5,16],anoth:[2,18],apikei:0,append:[0,16,18],appliabl:13,archiv:10,argc:6,argument:[0,2,10,18],argv:6,aspect:9,assess:5,asset:[1,2,5,6,7,8,9,14,15,18,21],asset_id:[1,14],assum:[6,14],assur:11,attribut:5,author:13,autom:20,automat:2,avail:[5,8],averag:[7,16],average_1:14,average_2:14,average_occur:7,avg1:18,avg2:18,avg:6,avoid:[5,14],awai:20,back:2,basic:0,bat:10,bear:14,becaus:10,befor:2,begin:[8,15],begun:8,behaviour:[1,14],below:[5,6,9,14,20],best:[1,14],beta:[1,14],between:[0,9,14,20],beyond:9,bia:14,binari:15,bind:0,binomi:7,blabla:10,black:[1,14],blackschol:[1,14],blibli:10,bloblo:10,bool:[1,12,14],both:18,bui:[2,18,20],bull:14,buy_hedg:2,buy_hedged_asym:2,calcul:[1,14],calculu:14,call:[0,1,6,14],can:[0,1,2,5,6,7,9,11,15,16],capabl:11,caracterist:[2,14],care:9,carefulli:12,categori:9,cdf:14,choos:[0,2],click:10,client:[9,18],close:[2,5,8,16,21],close_histori:21,close_posit:2,code:[],com:[0,18],come:[11,14],commun:9,compar:21,compil:[6,10],complex:12,compon:12,compos:[12,20],comput:[0,1,2,6,9,12,14,18,21],compute_greek:[1,14],concept:0,concern:10,concurr:5,condit:5,configur:2,conjunct:9,consist:10,constant:14,construct:16,constructor:[0,1,2,8,13,14,15,16,17,21],contain:[0,1,5,7,8,9,12,13,14,15,16,20,21],content:[5,13,18],context:[1,5,14],contract:[1,2,14],conveni:21,convienc:15,coordin:14,core:[0,2],cout:[6,18],covari:14,covariance_q:14,cqueue:5,craft:[5,15],creat:[0,1,5,9,14,15],cross:[14,20],cumul:14,currenc:14,current:[1,14,18,20],current_ds1:18,current_ds2:18,cvector:5,dai:[6,8,18],databas:5,dataset:[7,14,15,16,20],dataset_s:14,datasourc:[4,8],date:[13,17,21],decai:5,decis:18,declar:[5,18],declin:18,decompress:10,deduc:[0,18],dedud:[4,17],deep:9,deeper:9,defin:[2,21],degre:14,delta:[1,14],densiti:14,depend:[10,14],descend:17,describ:11,descript:[5,9],destructor:[0,1,8,13,14,15,16,17,21],detail:19,detect:[0,12,14],develop:[0,12,19],deviat:[6,7,14],differ:[0,7,20],directori:10,displai:[5,6],divid:14,dkjsksjd:18,document:0,doe:12,doesn:10,doest:0,don:18,dow:[6,18],dow_data:6,dow_max:6,dow_mean:6,dow_min:6,dow_sigma:6,down_sampl:[16,18],download:10,downsampl:16,dquot:18,draw:20,each:[7,9,14,18],earli:20,easi:10,educ:18,effici:9,either:16,element:[15,16],elimit:14,els:18,ema100:14,ema:[14,20],embed:5,embedd:[1,2],empti:[0,10],end:[14,15],endl:[6,18],endpoint:[0,18],entri:16,environ:[10,19],epoch:[5,8,15],equat:14,equiti:18,especi:[5,9],estim:14,etc:[0,5,9,21],evalu:18,even:14,event:7,everag:7,everi:[5,18],everyth:10,exact:15,exactli:6,exot:[1,14],expect:[9,14],expected_profit:14,experi:7,expexted_risk_reward:2,expir:[1,14],explain:[5,19],exponenti:14,exponential_moving_averag:14,exprim:[14,16],extra:7,extrac:15,facilit:20,fairi:10,fake:[6,18],fals:[12,14],far:11,fast:9,feed:4,felt:12,fetch:[4,5,8,9,17,18,21],few:10,file:10,filesquoteksdkinclud:[],filesquoteksdklib:[],financ:4,financi:[5,7,9],find:[0,6,9,15],first:[15,20],fit:7,follow:[5,6,10,11,19],form:[0,12,16,21],formal:12,format:[0,5,7,8,15,21],found:15,from:[1,4,5,6,8,9,12,14,15,20,21],fucntion:14,full:[0,5,18],further:[0,5],futur:9,gain:20,gamm:[1,14],gamma:14,gener:[],genet:5,get:18,get_data:[16,18],get_spread:16,get_timestamp:16,get_valu:16,give:[1,14],given:[2,10,14,15],good:[1,14,20],got:18,graph:[14,20],greek:[1,14],greeks_:[1,14],grow:11,guess:[0,12],half:14,have:[1,2,5,6,7,8,10,14,18],header:10,hedg:2,hedged_asset:2,hedged_limit:2,hedged_quant:2,hedged_stop:2,hello:6,help:[0,9,12,19],here:[0,2,9,18],high:[5,8,16,21],higher:14,highest:[8,15],histor:[4,5,8,9,21],hl2:16,host:0,how:[5,16,19],hpp:[4,5,6,14,18],html:0,http:[0,18],implement:5,impli:[1,14],implied_volatil:[1,14],includ:[4,5,6,10,14,18],index:[6,15],indic:[4,9,14,20,21],info:5,inform:[5,8],inherit:[1,14],initi:12,input:10,insid:[4,6,7,10,15,17],insight:20,installer_linux:10,installer_win64:10,instanc:[1,9,14],instanci:1,instead:14,integ:14,intend:14,intercept:14,interest:[1,14],intern:5,interract:[0,2],interv:[14,15,16],invest:18,involv:14,iostream:[6,18],is_in_monei:[1,14],iter:[11,15],jone:[6,18],journal:18,just:6,kei:[0,18],kept:16,kind:[5,7,8,12,14],know:[10,14],languag:0,largest:14,larn:0,last:[14,15,16,18],launch:10,law:7,layer:7,lcurl:10,least:14,left:16,let:0,leverag:14,lib:10,libcurl:10,libquotek:10,librari:10,like:[0,5,6],limit:2,line:[10,14,20],linear:14,linear_regress:14,link:2,linker:10,list:0,local:[5,8],locat:[6,10],log:7,lognorm:7,longer:[5,8],lose:20,loss:[2,18],low:[5,8,16,21],lowest:[8,15],lpthread:10,lqkslqskql:18,lquotek:10,macd:[9,20],machin:0,mai:18,main:[5,6,13,18],mainli:[7,9,12],make:[6,9,14,15,16],manag:2,mandatori:5,mani:5,map:0,market:[],max:[6,7,14,15],maximum:[6,7,15],mean:[6,7,14,20],meant:[1,2,4,5,8,9,12,14,20],member:[1,8,12,13,14,15],merg:6,meta:4,method:[0,2,5,14,15,16],might:18,min:[6,7,14,15],minimum:[6,7,15],minut:18,miss:18,misus:18,mix:12,model:[1,7,9,14],modelis:[1,14],modelized_opt:[1,14],modifi:10,momentum:20,monei:[1,2,14],more:[1,5,6,8,14],most:[0,7,9],mostli:[1,14],move:[],moving_averag:14,moving_average_convergeance_diverg:20,much:9,multipl:4,must:[0,7,14,18],name:[0,2],namespac:4,nb_new:17,need:[1,5,9,10,14],neg:[12,15,18],neg_sent:18,negat:12,neutral:14,next:[11,18],non:8,none:14,normal:[6,7,14,18],normal_cumulative_distribut:14,note:[0,1,5,9,10,14,15,16,20],noth:6,notic:11,now:9,number:[2,7,14,17],object:[0,1,2,5,12,14,16,17,18],occur:7,offer:[5,15],often:5,ohlc4:16,oieeoidj:18,onc:6,onli:[0,1,5,9,10,14,15,18],open:[2,5,8,16,21],oper:16,opportun:18,opposit:15,opt:10,option:0,order:[2,4,5,9,12,14,17,18],ordinari:14,other:[14,18,20],otherwis:14,our:[6,7,10,19],out:[1,14],output:6,over:16,overal:[12,18],paramet:[0,1,2,7,14,15,16,17,20,21],parent:2,pars:0,part:14,partial:0,pass:[0,17],path:10,payload:0,per:[6,14],percent:14,percent_delta:[14,18],percentag:14,perdiod:20,perform:[0,12,14,15,21],period:[8,14,16,18,20],periods_long_ema:20,periods_short_ema:20,periods_signal_lin:20,persiv:12,perspect:[9,14],phi:14,place:[2,10,14,15],pleas:[1,5,10,14],plenti:9,point:[14,16],poisson:7,polynom:14,polynomi:14,polynomial_regress:14,popul:14,portfolio:14,pos_sent:18,posit:[2,4,9,12,17,18],post:0,potenti:[4,14,17,18],pre:14,preambl:11,prec:14,prefix:0,present:7,previou:14,price:[7,8,16,21],probabl:[1,7,14],probali:7,problem:14,procedur:10,process:[0,4,5,9,12,17],profit:[2,14],program:[],project:10,promis:9,properli:14,properti:[6,10],protit:14,prove:7,provid:[0,1,2,5,8,12,14,18],pthread:10,publish:13,purpos:[5,9,18],put:[1,14],pwsh:[],quandl:5,quantit:[],quantiti:2,queri:[4,21],queue:5,quick:14,quot:[5,8],quotat:[6,8,18],quotek:[0,2,4],race:5,rand:[6,7,18],randoml:7,rang:16,rank:0,rate:[1,14],ratio:[2,14],raw:[0,5,8],read:5,readi:10,real:[5,8,15,18],realist:6,realli:[7,9],rec:[14,15,20],recent:14,recs1:14,recs2:14,reduc:[5,16],refer:14,reformat:15,regress:14,regroup:9,relashionship:0,relat:[0,9,12,13],relationship:[0,12],remind:14,remot:[5,8,9],remove_refer:5,replic:15,repres:[5,14,18],requir:[0,14],resampl:15,respons:[0,12,18],rest:11,result:[6,7,12,14,16,18],retriev:[4,16],reward:[2,14],rho:14,right:10,risk:[2,14],risk_reward_ratio:14,root:10,rsi:9,rss:4,rule:2,run:[10,18],saa:[9,10],safe:[1,5,14],safe_interest_r:[1,14],sai:6,same:[2,18],scale:[],scheme:[5,9],schole:1,score:12,script:10,sdk:[],search:[10,15],search_timestamp:15,second:[1,5,6,14,16,18,20],section:[5,6,10,11,19],secur:2,see:18,select:10,sell:[2,18,20],send:[0,2,9,18],sens:16,sent:[0,18],sentenc:12,sentiment:[0,12],sequenc:14,serv:9,server:[0,10],servic:[0,4,9,18,21],set:[7,9,12,20],setup:[10,19],shape:14,share:5,shole:[1,14],should:12,show:9,side:[18,20],sigma:[6,7,18],signal:[18,20],signific:11,simpl:[6,14,15],simpli:16,simul:[6,18],sinc:[0,9,10],singl:14,situat:18,size:[7,15,16,18],size_t:16,skip:[],slope:14,sma20:18,sma50:14,sma:[14,18],small:20,smallest:14,smartbui:2,smartsel:2,snd:14,solut:10,some:[0,1,6,9,18],someth:[1,14,18],sort:17,special:[5,14,15],spread:[5,15,16],squar:14,standard:[6,7,14,20],standard_devi:14,standard_normal_distribut:14,start:15,start_offset:15,start_offsret:15,statist:9,std:[0,1,2,5,6,7,12,13,14,15,16,17,18,20,21],step:[],stock:4,stop:[2,14],storagetyp:5,store:[1,5,8,9,12,13,14,15],strategi:[0,2,4,9,17,18],stream:17,street:18,strength:20,strike:[1,14],string:[0,1,2,12,13,14,16,17,21],struct:[5,12,14],studio:10,subject:12,subset:5,subvector:15,succe:7,success_prob:7,suggest:20,summer:[0,12],support:[0,1,14],susual:20,symbol:21,system:10,systemat:16,tag:13,take:[2,4,9,12,15,16,17,18],target:0,tell:[0,1,12,14],templat:5,tens:12,test:14,text:0,than:[2,14],thei:[5,9,16,18],them:[0,4,9,10,17],therefor:[0,16,18],thereshold:[2,14],theta:14,thi:[1,5,9,10,11,12,14,16,18,20],thing:[10,18],think:9,thread:5,three:5,through:1,tick:[5,6,8,16,18],ticket:2,ticket_id:2,tima_as_valu:15,time_as_valu:15,time_inf:15,time_sup:15,timestamp:[5,8,13,15,16],timestamps_export:15,titl:13,togeth:14,toolchain:10,trade:[0,2,9,14],trader:9,trend:[14,20],trend_p:14,trend_percentag:14,tri:7,trinom:14,trivial:12,troublesom:14,tupl:16,two:14,type:[1,5,14],typedef:[5,14],typenam:5,typic:[5,10,16,18],under:[0,21],underli:[1,7,14],underlying_pric:[1,14],uniform:7,uniformli:7,unzip:10,uri:0,url:[0,18],usabl:5,user:[0,2],usual:[5,8,10,15,20],valu:[5,6,7,8,14,15,16,18,20,21],values_export:15,values_import:15,vanilla:[1,14],varianc:14,variance_q:14,variat:14,variou:5,vector:[0,5,6,7,13,14,15,16,17,18,20,21],vega:14,verb:12,veri:[0,5,6,11,21],version:14,visual:10,vital:5,volatil:[1,14],wai:[1,11,14,18],wall:18,want:[1,6,7,14],weight:14,weighted_averag:14,weighted_moving_averag:14,were:5,wether:[0,1,14],what:0,when:[9,10,11,14,20],where:[2,10,14],whether:12,which:[0,1,4,5,6,7,8,9,10,12,13,14,15,17,18,21],who:[0,12],whole:0,why:5,width:6,within:[1,14],without:0,wma:14,work:[5,14,16],world:6,worth:15,would:16,wrapper:7,write:0,wsj:18,wsj_list:[],wsj_nlist:18,www:[0,18],xml:18,yahoo:4,yahoofin:[4,21],year:[1,14],yet:6,you:[0,1,2,5,6,9,10,11,12,14,19],your:[5,9,10,14,19],your_c:10,your_program:10,yyyi:21},titles:["Alchemy API","Options pricing","Broker API","Broker Interactions","Data Sources","Data Structures","First Program","Random Distributions Generators","History","Welcome to Quotek SDK documentation!","Install","Machine Learning","Data Structures","News","Quantitative Analysis","Record","Records","RSS Feeds","SDK Examples","Quick Start","Technical Analysis","Yahoo! Finance"],titleterms:{"class":13,"new":13,alchemi:[0,18],analysi:[9,14,18,20],api:[0,2,18],averag:18,begin:6,broker:[2,3,9],build:10,code:6,content:19,data:[4,5,9,12],datasourc:9,disclaim:18,distribut:7,document:[5,9,13],down:5,evolut:18,exampl:18,extract:5,feed:17,financ:21,first:6,full:6,gener:7,get:19,histori:[5,8],instal:10,interact:3,interract:9,learn:[9,11],let:6,link:10,linux:10,machin:[9,11],market:18,move:18,multipl:18,namespac:5,option:1,predic:18,price:1,program:6,quant:9,quantit:14,quick:19,qunatit:9,quotek:[5,9],random:7,record:[5,15,16],rss:17,sampl:5,scale:18,sdk:[9,18],sentiment:18,seri:5,sourc:[4,6,9],start:19,stock:18,structur:[5,9,12],tabl:19,technic:[9,20],text:5,time:[5,18],welcom:9,what:9,window:10,yahoo:21}})