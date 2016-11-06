enum HOSEI_CHECK{EASYCHECK_PASS,EASYCHECK_FAIL};
enum HOSEI_GROUPCHECK{SAMEGROUP,DIFFGROUP,SAMEHAI};
enum HOSEI_JUDGE_ENABLE{HOSEI_ENABLE,HOSEI_DISABLE};
enum HOSEI_WAY{WAY_LEFT,WAY_RIGHT};
enum HOSEI_FLAG{HOSEI_NAKI,HOSEI_TEHAI};
enum HOSEI_MODE{HOSEI_MODE_JIHAI,HOSEI_MODE_ALL,HOSEI_MODE_SAMEHAI,HOSEI_MODE_SAMEGR,HOSEI_MODE_DIFFGR_LEFT,HOSEI_MODE_DIFFGR_RIGHT};


int fukeisan_main(jboolean *arrCondition, jbyte *arrTehai, jbyte *arrDora, jbyte *arrNakihai, jboolean *arrNakiflag,jint honba, jint *arrResult, jboolean *arrYakulist);
int correctionAct(jboolean *arrCondition, jbyte *arrTehai, jbyte *arrDora, jbyte *arrNakihai, jboolean *arrNakiflag,jint honba, jint *arrResult, jboolean *arrYakulist);

