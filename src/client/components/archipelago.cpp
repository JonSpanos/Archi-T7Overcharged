#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "havok/hks_api.hpp"
#include "havok/lua_api.hpp"

#define ASIO_STANDALONE

#define UUID_FILE "uuid"

#define VERSION_TUPLE {0,4,3}

#include <windows.h>

#include <apclient.hpp>
#include <apuuid.hpp>
#include "console.hpp"


using nlohmann::json;

namespace archipelago
{

	
	const std::string GAME_NAME = "Black Ops 3 - Zombies";
	
	//Full Remote Items
	const int items_handling = 0b111;

	const int64_t loc_error = (int64_t)(-1);
	
	APClient* ap = nullptr;

	//For the Lobby Connection
	APClient* temp_ap = nullptr;

	bool socket_connected = false;

	bool ap_sync_queued = false;

	bool awaitingReconnect = false;

	int baseID = 0;

	int lastItem = 0;

	std::string slot = "";
	std::string seed = "";

	std::list<int64_t> checkedLocationsList = { };
	std::list<int64_t> scoutedLocationsList = { };

	static std::unordered_map<std::string, int> loc_name_to_id{
		{"(SoE) Round 01",1},
		{"(SoE) Round 02",2},
		{"(SoE) Round 03",3},
		{"(SoE) Round 04",4},
		{"(SoE) Round 05",5},
		{"(SoE) Round 06",6},
		{"(SoE) Round 07",7},
		{"(SoE) Round 08",8},
		{"(SoE) Round 09",9},
		{"(SoE) Round 10",10},
		{"(SoE) Round 11",11},
		{"(SoE) Round 12",12},
		{"(SoE) Round 13",13},
		{"(SoE) Round 14",14},
		{"(SoE) Round 15",15},
		{"(SoE) Round 16",16},
		{"(SoE) Round 17",17},
		{"(SoE) Round 18",18},
		{"(SoE) Round 19",19},
		{"(SoE) Round 20",20},
		{"(SoE) Round 21",21},
		{"(SoE) Round 22",22},
		{"(SoE) Round 23",23},
		{"(SoE) Round 24",24},
		{"(SoE) Round 25",25},
		{"(SoE) Round 26",26},
		{"(SoE) Round 27",27},
		{"(SoE) Round 28",28},
		{"(SoE) Round 29",29},
		{"(SoE) Round 30",30},
		{"(SoE) Round 31",31},
		{"(SoE) Round 32",32},
		{"(SoE) Round 33",33},
		{"(SoE) Round 34",34},
		{"(SoE) Round 35",35},
		{"(SoE) Round 36",36},
		{"(SoE) Round 37",37},
		{"(SoE) Round 38",38},
		{"(SoE) Round 39",39},
		{"(SoE) Round 40",40},
		{"(SoE) Round 41",41},
		{"(SoE) Round 42",42},
		{"(SoE) Round 43",43},
		{"(SoE) Round 44",44},
		{"(SoE) Round 45",45},
		{"(SoE) Round 46",46},
		{"(SoE) Round 47",47},
		{"(SoE) Round 48",48},
		{"(SoE) Round 49",49},
		{"(SoE) Round 50",50},
		{"(SoE) Round 51",51},
		{"(SoE) Round 52",52},
		{"(SoE) Round 53",53},
		{"(SoE) Round 54",54},
		{"(SoE) Round 55",55},
		{"(SoE) Round 56",56},
		{"(SoE) Round 57",57},
		{"(SoE) Round 58",58},
		{"(SoE) Round 59",59},
		{"(SoE) Round 60",60},
		{"(SoE) Round 61",61},
		{"(SoE) Round 62",62},
		{"(SoE) Round 63",63},
		{"(SoE) Round 64",64},
		{"(SoE) Round 65",65},
		{"(SoE) Round 66",66},
		{"(SoE) Round 67",67},
		{"(SoE) Round 68",68},
		{"(SoE) Round 69",69},
		{"(SoE) Round 70",70},
		{"(SoE) Round 71",71},
		{"(SoE) Round 72",72},
		{"(SoE) Round 73",73},
		{"(SoE) Round 74",74},
		{"(SoE) Round 75",75},
		{"(SoE) Round 76",76},
		{"(SoE) Round 77",77},
		{"(SoE) Round 78",78},
		{"(SoE) Round 79",79},
		{"(SoE) Round 80",80},
		{"(SoE) Round 81",81},
		{"(SoE) Round 82",82},
		{"(SoE) Round 83",83},
		{"(SoE) Round 84",84},
		{"(SoE) Round 85",85},
		{"(SoE) Round 86",86},
		{"(SoE) Round 87",87},
		{"(SoE) Round 88",88},
		{"(SoE) Round 89",89},
		{"(SoE) Round 90",90},
		{"(SoE) Round 91",91},
		{"(SoE) Round 92",92},
		{"(SoE) Round 93",93},
		{"(SoE) Round 94",94},
		{"(SoE) Round 95",95},
		{"(SoE) Round 96",96},
		{"(SoE) Round 97",97},
		{"(SoE) Round 98",98},
		{"(SoE) Round 99",99},
		{"(SoE) Round 100",100},
		{"(The Giant) Round 01",101},
		{"(The Giant) Round 02",102},
		{"(The Giant) Round 03",103},
		{"(The Giant) Round 04",104},
		{"(The Giant) Round 05",105},
		{"(The Giant) Round 06",106},
		{"(The Giant) Round 07",107},
		{"(The Giant) Round 08",108},
		{"(The Giant) Round 09",109},
		{"(The Giant) Round 10",110},
		{"(The Giant) Round 11",111},
		{"(The Giant) Round 12",112},
		{"(The Giant) Round 13",113},
		{"(The Giant) Round 14",114},
		{"(The Giant) Round 15",115},
		{"(The Giant) Round 16",116},
		{"(The Giant) Round 17",117},
		{"(The Giant) Round 18",118},
		{"(The Giant) Round 19",119},
		{"(The Giant) Round 20",120},
		{"(The Giant) Round 21",121},
		{"(The Giant) Round 22",122},
		{"(The Giant) Round 23",123},
		{"(The Giant) Round 24",124},
		{"(The Giant) Round 25",125},
		{"(The Giant) Round 26",126},
		{"(The Giant) Round 27",127},
		{"(The Giant) Round 28",128},
		{"(The Giant) Round 29",129},
		{"(The Giant) Round 30",130},
		{"(The Giant) Round 31",131},
		{"(The Giant) Round 32",132},
		{"(The Giant) Round 33",133},
		{"(The Giant) Round 34",134},
		{"(The Giant) Round 35",135},
		{"(The Giant) Round 36",136},
		{"(The Giant) Round 37",137},
		{"(The Giant) Round 38",138},
		{"(The Giant) Round 39",139},
		{"(The Giant) Round 40",140},
		{"(The Giant) Round 41",141},
		{"(The Giant) Round 42",142},
		{"(The Giant) Round 43",143},
		{"(The Giant) Round 44",144},
		{"(The Giant) Round 45",145},
		{"(The Giant) Round 46",146},
		{"(The Giant) Round 47",147},
		{"(The Giant) Round 48",148},
		{"(The Giant) Round 49",149},
		{"(The Giant) Round 50",150},
		{"(The Giant) Round 51",151},
		{"(The Giant) Round 52",152},
		{"(The Giant) Round 53",153},
		{"(The Giant) Round 54",154},
		{"(The Giant) Round 55",155},
		{"(The Giant) Round 56",156},
		{"(The Giant) Round 57",157},
		{"(The Giant) Round 58",158},
		{"(The Giant) Round 59",159},
		{"(The Giant) Round 60",160},
		{"(The Giant) Round 61",161},
		{"(The Giant) Round 62",162},
		{"(The Giant) Round 63",163},
		{"(The Giant) Round 64",164},
		{"(The Giant) Round 65",165},
		{"(The Giant) Round 66",166},
		{"(The Giant) Round 67",167},
		{"(The Giant) Round 68",168},
		{"(The Giant) Round 69",169},
		{"(The Giant) Round 70",170},
		{"(The Giant) Round 71",171},
		{"(The Giant) Round 72",172},
		{"(The Giant) Round 73",173},
		{"(The Giant) Round 74",174},
		{"(The Giant) Round 75",175},
		{"(The Giant) Round 76",176},
		{"(The Giant) Round 77",177},
		{"(The Giant) Round 78",178},
		{"(The Giant) Round 79",179},
		{"(The Giant) Round 80",180},
		{"(The Giant) Round 81",181},
		{"(The Giant) Round 82",182},
		{"(The Giant) Round 83",183},
		{"(The Giant) Round 84",184},
		{"(The Giant) Round 85",185},
		{"(The Giant) Round 86",186},
		{"(The Giant) Round 87",187},
		{"(The Giant) Round 88",188},
		{"(The Giant) Round 89",189},
		{"(The Giant) Round 90",190},
		{"(The Giant) Round 91",191},
		{"(The Giant) Round 92",192},
		{"(The Giant) Round 93",193},
		{"(The Giant) Round 94",194},
		{"(The Giant) Round 95",195},
		{"(The Giant) Round 96",196},
		{"(The Giant) Round 97",197},
		{"(The Giant) Round 98",198},
		{"(The Giant) Round 99",199},
		{"(The Giant) Round 100",200},
		{ "(DE) Round 01",201 },
		{ "(DE) Round 02",202 },
		{ "(DE) Round 03",203 },
		{ "(DE) Round 04",204 },
		{ "(DE) Round 05",205 },
		{ "(DE) Round 06",206 },
		{ "(DE) Round 07",207 },
		{ "(DE) Round 08",208 },
		{ "(DE) Round 09",209 },
		{ "(DE) Round 10",210 },
		{ "(DE) Round 11",211 },
		{ "(DE) Round 12",212 },
		{ "(DE) Round 13",213 },
		{ "(DE) Round 14",214 },
		{ "(DE) Round 15",215 },
		{ "(DE) Round 16",216 },
		{ "(DE) Round 17",217 },
		{ "(DE) Round 18",218 },
		{ "(DE) Round 19",219 },
		{ "(DE) Round 20",220 },
		{ "(DE) Round 21",221 },
		{ "(DE) Round 22",222 },
		{ "(DE) Round 23",223 },
		{ "(DE) Round 24",224 },
		{ "(DE) Round 25",225 },
		{ "(DE) Round 26",226 },
		{ "(DE) Round 27",227 },
		{ "(DE) Round 28",228 },
		{ "(DE) Round 29",229 },
		{ "(DE) Round 30",230 },
		{ "(DE) Round 31",231 },
		{ "(DE) Round 32",232 },
		{ "(DE) Round 33",233 },
		{ "(DE) Round 34",234 },
		{ "(DE) Round 35",235 },
		{ "(DE) Round 36",236 },
		{ "(DE) Round 37",237 },
		{ "(DE) Round 38",238 },
		{ "(DE) Round 39",239 },
		{ "(DE) Round 40",240 },
		{ "(DE) Round 41",241 },
		{ "(DE) Round 42",242 },
		{ "(DE) Round 43",243 },
		{ "(DE) Round 44",244 },
		{ "(DE) Round 45",245 },
		{ "(DE) Round 46",246 },
		{ "(DE) Round 47",247 },
		{ "(DE) Round 48",248 },
		{ "(DE) Round 49",249 },
		{ "(DE) Round 50",250 },
		{ "(DE) Round 51",251 },
		{ "(DE) Round 52",252 },
		{ "(DE) Round 53",253 },
		{ "(DE) Round 54",254 },
		{ "(DE) Round 55",255 },
		{ "(DE) Round 56",256 },
		{ "(DE) Round 57",257 },
		{ "(DE) Round 58",258 },
		{ "(DE) Round 59",259 },
		{ "(DE) Round 60",260 },
		{ "(DE) Round 61",261 },
		{ "(DE) Round 62",262 },
		{ "(DE) Round 63",263 },
		{ "(DE) Round 64",264 },
		{ "(DE) Round 65",265 },
		{ "(DE) Round 66",266 },
		{ "(DE) Round 67",267 },
		{ "(DE) Round 68",268 },
		{ "(DE) Round 69",269 },
		{ "(DE) Round 70",270 },
		{ "(DE) Round 71",271 },
		{ "(DE) Round 72",272 },
		{ "(DE) Round 73",273 },
		{ "(DE) Round 74",274 },
		{ "(DE) Round 75",275 },
		{ "(DE) Round 76",276 },
		{ "(DE) Round 77",277 },
		{ "(DE) Round 78",278 },
		{ "(DE) Round 79",279 },
		{ "(DE) Round 80",280 },
		{ "(DE) Round 81",281 },
		{ "(DE) Round 82",282 },
		{ "(DE) Round 83",283 },
		{ "(DE) Round 84",284 },
		{ "(DE) Round 85",285 },
		{ "(DE) Round 86",286 },
		{ "(DE) Round 87",287 },
		{ "(DE) Round 88",288 },
		{ "(DE) Round 89",289 },
		{ "(DE) Round 90",290 },
		{ "(DE) Round 91",291 },
		{ "(DE) Round 92",292 },
		{ "(DE) Round 93",293 },
		{ "(DE) Round 94",294 },
		{ "(DE) Round 95",295 },
		{ "(DE) Round 96",296 },
		{ "(DE) Round 97",297 },
		{ "(DE) Round 98",298 },
		{ "(DE) Round 99",299 },
		{ "(DE) Round 100",300 },
		{ "(ZnS) Round 01",301 },
		{ "(ZnS) Round 02",302 },
		{ "(ZnS) Round 03",303 },
		{ "(ZnS) Round 04",304 },
		{ "(ZnS) Round 05",305 },
		{ "(ZnS) Round 06",306 },
		{ "(ZnS) Round 07",307 },
		{ "(ZnS) Round 08",308 },
		{ "(ZnS) Round 09",309 },
		{ "(ZnS) Round 10",310 },
		{ "(ZnS) Round 11",311 },
		{ "(ZnS) Round 12",312 },
		{ "(ZnS) Round 13",313 },
		{ "(ZnS) Round 14",314 },
		{ "(ZnS) Round 15",315 },
		{ "(ZnS) Round 16",316 },
		{ "(ZnS) Round 17",317 },
		{ "(ZnS) Round 18",318 },
		{ "(ZnS) Round 19",319 },
		{ "(ZnS) Round 20",320 },
		{ "(ZnS) Round 21",321 },
		{ "(ZnS) Round 22",322 },
		{ "(ZnS) Round 23",323 },
		{ "(ZnS) Round 24",324 },
		{ "(ZnS) Round 25",325 },
		{ "(ZnS) Round 26",326 },
		{ "(ZnS) Round 27",327 },
		{ "(ZnS) Round 28",328 },
		{ "(ZnS) Round 29",329 },
		{ "(ZnS) Round 30",330 },
		{ "(ZnS) Round 31",331 },
		{ "(ZnS) Round 32",332 },
		{ "(ZnS) Round 33",333 },
		{ "(ZnS) Round 34",334 },
		{ "(ZnS) Round 35",335 },
		{ "(ZnS) Round 36",336 },
		{ "(ZnS) Round 37",337 },
		{ "(ZnS) Round 38",338 },
		{ "(ZnS) Round 39",339 },
		{ "(ZnS) Round 40",340 },
		{ "(ZnS) Round 41",341 },
		{ "(ZnS) Round 42",342 },
		{ "(ZnS) Round 43",343 },
		{ "(ZnS) Round 44",344 },
		{ "(ZnS) Round 45",345 },
		{ "(ZnS) Round 46",346 },
		{ "(ZnS) Round 47",347 },
		{ "(ZnS) Round 48",348 },
		{ "(ZnS) Round 49",349 },
		{ "(ZnS) Round 50",350 },
		{ "(ZnS) Round 51",351 },
		{ "(ZnS) Round 52",352 },
		{ "(ZnS) Round 53",353 },
		{ "(ZnS) Round 54",354 },
		{ "(ZnS) Round 55",355 },
		{ "(ZnS) Round 56",356 },
		{ "(ZnS) Round 57",357 },
		{ "(ZnS) Round 58",358 },
		{ "(ZnS) Round 59",359 },
		{ "(ZnS) Round 60",360 },
		{ "(ZnS) Round 61",361 },
		{ "(ZnS) Round 62",362 },
		{ "(ZnS) Round 63",363 },
		{ "(ZnS) Round 64",364 },
		{ "(ZnS) Round 65",365 },
		{ "(ZnS) Round 66",366 },
		{ "(ZnS) Round 67",367 },
		{ "(ZnS) Round 68",368 },
		{ "(ZnS) Round 69",369 },
		{ "(ZnS) Round 70",370 },
		{ "(ZnS) Round 71",371 },
		{ "(ZnS) Round 72",372 },
		{ "(ZnS) Round 73",373 },
		{ "(ZnS) Round 74",374 },
		{ "(ZnS) Round 75",375 },
		{ "(ZnS) Round 76",376 },
		{ "(ZnS) Round 77",377 },
		{ "(ZnS) Round 78",378 },
		{ "(ZnS) Round 79",379 },
		{ "(ZnS) Round 80",380 },
		{ "(ZnS) Round 81",381 },
		{ "(ZnS) Round 82",382 },
		{ "(ZnS) Round 83",383 },
		{ "(ZnS) Round 84",384 },
		{ "(ZnS) Round 85",385 },
		{ "(ZnS) Round 86",386 },
		{ "(ZnS) Round 87",387 },
		{ "(ZnS) Round 88",388 },
		{ "(ZnS) Round 89",389 },
		{ "(ZnS) Round 90",390 },
		{ "(ZnS) Round 91",391 },
		{ "(ZnS) Round 92",392 },
		{ "(ZnS) Round 93",393 },
		{ "(ZnS) Round 94",394 },
		{ "(ZnS) Round 95",395 },
		{ "(ZnS) Round 96",396 },
		{ "(ZnS) Round 97",397 },
		{ "(ZnS) Round 98",398 },
		{ "(ZnS) Round 99",399 },
		{ "(ZnS) Round 100",400 },
		{ "(GK) Round 01",401 },
		{ "(GK) Round 02",402 },
		{ "(GK) Round 03",403 },
		{ "(GK) Round 04",404 },
		{ "(GK) Round 05",405 },
		{ "(GK) Round 06",406 },
		{ "(GK) Round 07",407 },
		{ "(GK) Round 08",408 },
		{ "(GK) Round 09",409 },
		{ "(GK) Round 10",410 },
		{ "(GK) Round 11",411 },
		{ "(GK) Round 12",412 },
		{ "(GK) Round 13",413 },
		{ "(GK) Round 14",414 },
		{ "(GK) Round 15",415 },
		{ "(GK) Round 16",416 },
		{ "(GK) Round 17",417 },
		{ "(GK) Round 18",418 },
		{ "(GK) Round 19",419 },
		{ "(GK) Round 20",420 },
		{ "(GK) Round 21",421 },
		{ "(GK) Round 22",422 },
		{ "(GK) Round 23",423 },
		{ "(GK) Round 24",424 },
		{ "(GK) Round 25",425 },
		{ "(GK) Round 26",426 },
		{ "(GK) Round 27",427 },
		{ "(GK) Round 28",428 },
		{ "(GK) Round 29",429 },
		{ "(GK) Round 30",430 },
		{ "(GK) Round 31",431 },
		{ "(GK) Round 32",432 },
		{ "(GK) Round 33",433 },
		{ "(GK) Round 34",434 },
		{ "(GK) Round 35",435 },
		{ "(GK) Round 36",436 },
		{ "(GK) Round 37",437 },
		{ "(GK) Round 38",438 },
		{ "(GK) Round 39",439 },
		{ "(GK) Round 40",440 },
		{ "(GK) Round 41",441 },
		{ "(GK) Round 42",442 },
		{ "(GK) Round 43",443 },
		{ "(GK) Round 44",444 },
		{ "(GK) Round 45",445 },
		{ "(GK) Round 46",446 },
		{ "(GK) Round 47",447 },
		{ "(GK) Round 48",448 },
		{ "(GK) Round 49",449 },
		{ "(GK) Round 50",450 },
		{ "(GK) Round 51",451 },
		{ "(GK) Round 52",452 },
		{ "(GK) Round 53",453 },
		{ "(GK) Round 54",454 },
		{ "(GK) Round 55",455 },
		{ "(GK) Round 56",456 },
		{ "(GK) Round 57",457 },
		{ "(GK) Round 58",458 },
		{ "(GK) Round 59",459 },
		{ "(GK) Round 60",460 },
		{ "(GK) Round 61",461 },
		{ "(GK) Round 62",462 },
		{ "(GK) Round 63",463 },
		{ "(GK) Round 64",464 },
		{ "(GK) Round 65",465 },
		{ "(GK) Round 66",466 },
		{ "(GK) Round 67",467 },
		{ "(GK) Round 68",468 },
		{ "(GK) Round 69",469 },
		{ "(GK) Round 70",470 },
		{ "(GK) Round 71",471 },
		{ "(GK) Round 72",472 },
		{ "(GK) Round 73",473 },
		{ "(GK) Round 74",474 },
		{ "(GK) Round 75",475 },
		{ "(GK) Round 76",476 },
		{ "(GK) Round 77",477 },
		{ "(GK) Round 78",478 },
		{ "(GK) Round 79",479 },
		{ "(GK) Round 80",480 },
		{ "(GK) Round 81",481 },
		{ "(GK) Round 82",482 },
		{ "(GK) Round 83",483 },
		{ "(GK) Round 84",484 },
		{ "(GK) Round 85",485 },
		{ "(GK) Round 86",486 },
		{ "(GK) Round 87",487 },
		{ "(GK) Round 88",488 },
		{ "(GK) Round 89",489 },
		{ "(GK) Round 90",490 },
		{ "(GK) Round 91",491 },
		{ "(GK) Round 92",492 },
		{ "(GK) Round 93",493 },
		{ "(GK) Round 94",494 },
		{ "(GK) Round 95",495 },
		{ "(GK) Round 96",496 },
		{ "(GK) Round 97",497 },
		{ "(GK) Round 98",498 },
		{ "(GK) Round 99",499 },
		{ "(GK) Round 100",500},
		{ "(Rev) Round 01",501 },
		{ "(Rev) Round 02",502 },
		{ "(Rev) Round 03",503 },
		{ "(Rev) Round 04",504 },
		{ "(Rev) Round 05",505 },
		{ "(Rev) Round 06",506 },
		{ "(Rev) Round 07",507 },
		{ "(Rev) Round 08",508 },
		{ "(Rev) Round 09",509 },
		{ "(Rev) Round 10",510 },
		{ "(Rev) Round 11",511 },
		{ "(Rev) Round 12",512 },
		{ "(Rev) Round 13",513 },
		{ "(Rev) Round 14",514 },
		{ "(Rev) Round 15",515 },
		{ "(Rev) Round 16",516 },
		{ "(Rev) Round 17",517 },
		{ "(Rev) Round 18",518 },
		{ "(Rev) Round 19",519 },
		{ "(Rev) Round 20",520 },
		{ "(Rev) Round 21",521 },
		{ "(Rev) Round 22",522 },
		{ "(Rev) Round 23",523 },
		{ "(Rev) Round 24",524 },
		{ "(Rev) Round 25",525 },
		{ "(Rev) Round 26",526 },
		{ "(Rev) Round 27",527 },
		{ "(Rev) Round 28",528 },
		{ "(Rev) Round 29",529 },
		{ "(Rev) Round 30",530 },
		{ "(Rev) Round 31",531 },
		{ "(Rev) Round 32",532 },
		{ "(Rev) Round 33",533 },
		{ "(Rev) Round 34",534 },
		{ "(Rev) Round 35",535 },
		{ "(Rev) Round 36",536 },
		{ "(Rev) Round 37",537 },
		{ "(Rev) Round 38",538 },
		{ "(Rev) Round 39",539 },
		{ "(Rev) Round 40",540 },
		{ "(Rev) Round 41",541 },
		{ "(Rev) Round 42",542 },
		{ "(Rev) Round 43",543 },
		{ "(Rev) Round 44",544 },
		{ "(Rev) Round 45",545 },
		{ "(Rev) Round 46",546 },
		{ "(Rev) Round 47",547 },
		{ "(Rev) Round 48",548 },
		{ "(Rev) Round 49",549 },
		{ "(Rev) Round 50",550 },
		{ "(Rev) Round 51",551 },
		{ "(Rev) Round 52",552 },
		{ "(Rev) Round 53",553 },
		{ "(Rev) Round 54",554 },
		{ "(Rev) Round 55",555 },
		{ "(Rev) Round 56",556 },
		{ "(Rev) Round 57",557 },
		{ "(Rev) Round 58",558 },
		{ "(Rev) Round 59",559 },
		{ "(Rev) Round 60",560 },
		{ "(Rev) Round 61",561 },
		{ "(Rev) Round 62",562 },
		{ "(Rev) Round 63",563 },
		{ "(Rev) Round 64",564 },
		{ "(Rev) Round 65",565 },
		{ "(Rev) Round 66",566 },
		{ "(Rev) Round 67",567 },
		{ "(Rev) Round 68",568 },
		{ "(Rev) Round 69",569 },
		{ "(Rev) Round 70",570 },
		{ "(Rev) Round 71",571 },
		{ "(Rev) Round 72",572 },
		{ "(Rev) Round 73",573 },
		{ "(Rev) Round 74",574 },
		{ "(Rev) Round 75",575 },
		{ "(Rev) Round 76",576 },
		{ "(Rev) Round 77",577 },
		{ "(Rev) Round 78",578 },
		{ "(Rev) Round 79",579 },
		{ "(Rev) Round 80",580 },
		{ "(Rev) Round 81",581 },
		{ "(Rev) Round 82",582 },
		{ "(Rev) Round 83",583 },
		{ "(Rev) Round 84",584 },
		{ "(Rev) Round 85",585 },
		{ "(Rev) Round 86",586 },
		{ "(Rev) Round 87",587 },
		{ "(Rev) Round 88",588 },
		{ "(Rev) Round 89",589 },
		{ "(Rev) Round 90",590 },
		{ "(Rev) Round 91",591 },
		{ "(Rev) Round 92",592 },
		{ "(Rev) Round 93",593 },
		{ "(Rev) Round 94",594 },
		{ "(Rev) Round 95",595 },
		{ "(Rev) Round 96",596 },
		{ "(Rev) Round 97",597 },
		{ "(Rev) Round 98",598 },
		{ "(Rev) Round 99",599 },
		{ "(Rev) Round 100",600 },
		{ "(Nacht) Round 01",601 },
		{ "(Nacht) Round 02",602 },
		{ "(Nacht) Round 03",603 },
		{ "(Nacht) Round 04",604 },
		{ "(Nacht) Round 05",605 },
		{ "(Nacht) Round 06",606 },
		{ "(Nacht) Round 07",607 },
		{ "(Nacht) Round 08",608 },
		{ "(Nacht) Round 09",609 },
		{ "(Nacht) Round 10",610 },
		{ "(Nacht) Round 11",611 },
		{ "(Nacht) Round 12",612 },
		{ "(Nacht) Round 13",613 },
		{ "(Nacht) Round 14",614 },
		{ "(Nacht) Round 15",615 },
		{ "(Nacht) Round 16",616 },
		{ "(Nacht) Round 17",617 },
		{ "(Nacht) Round 18",618 },
		{ "(Nacht) Round 19",619 },
		{ "(Nacht) Round 20",620 },
		{ "(Nacht) Round 21",621 },
		{ "(Nacht) Round 22",622 },
		{ "(Nacht) Round 23",623 },
		{ "(Nacht) Round 24",624 },
		{ "(Nacht) Round 25",625 },
		{ "(Nacht) Round 26",626 },
		{ "(Nacht) Round 27",627 },
		{ "(Nacht) Round 28",628 },
		{ "(Nacht) Round 29",629 },
		{ "(Nacht) Round 30",630 },
		{ "(Nacht) Round 31",631 },
		{ "(Nacht) Round 32",632 },
		{ "(Nacht) Round 33",633 },
		{ "(Nacht) Round 34",634 },
		{ "(Nacht) Round 35",635 },
		{ "(Nacht) Round 36",636 },
		{ "(Nacht) Round 37",637 },
		{ "(Nacht) Round 38",638 },
		{ "(Nacht) Round 39",639 },
		{ "(Nacht) Round 40",640 },
		{ "(Nacht) Round 41",641 },
		{ "(Nacht) Round 42",642 },
		{ "(Nacht) Round 43",643 },
		{ "(Nacht) Round 44",644 },
		{ "(Nacht) Round 45",645 },
		{ "(Nacht) Round 46",646 },
		{ "(Nacht) Round 47",647 },
		{ "(Nacht) Round 48",648 },
		{ "(Nacht) Round 49",649 },
		{ "(Nacht) Round 50",650 },
		{ "(Nacht) Round 51",651 },
		{ "(Nacht) Round 52",652 },
		{ "(Nacht) Round 53",653 },
		{ "(Nacht) Round 54",654 },
		{ "(Nacht) Round 55",655 },
		{ "(Nacht) Round 56",656 },
		{ "(Nacht) Round 57",657 },
		{ "(Nacht) Round 58",658 },
		{ "(Nacht) Round 59",659 },
		{ "(Nacht) Round 60",660 },
		{ "(Nacht) Round 61",661 },
		{ "(Nacht) Round 62",662 },
		{ "(Nacht) Round 63",663 },
		{ "(Nacht) Round 64",664 },
		{ "(Nacht) Round 65",665 },
		{ "(Nacht) Round 66",666 },
		{ "(Nacht) Round 67",667 },
		{ "(Nacht) Round 68",668 },
		{ "(Nacht) Round 69",669 },
		{ "(Nacht) Round 70",670 },
		{ "(Nacht) Round 71",671 },
		{ "(Nacht) Round 72",672 },
		{ "(Nacht) Round 73",673 },
		{ "(Nacht) Round 74",674 },
		{ "(Nacht) Round 75",675 },
		{ "(Nacht) Round 76",676 },
		{ "(Nacht) Round 77",677 },
		{ "(Nacht) Round 78",678 },
		{ "(Nacht) Round 79",679 },
		{ "(Nacht) Round 80",680 },
		{ "(Nacht) Round 81",681 },
		{ "(Nacht) Round 82",682 },
		{ "(Nacht) Round 83",683 },
		{ "(Nacht) Round 84",684 },
		{ "(Nacht) Round 85",685 },
		{ "(Nacht) Round 86",686 },
		{ "(Nacht) Round 87",687 },
		{ "(Nacht) Round 88",688 },
		{ "(Nacht) Round 89",689 },
		{ "(Nacht) Round 90",690 },
		{ "(Nacht) Round 91",691 },
		{ "(Nacht) Round 92",692 },
		{ "(Nacht) Round 93",693 },
		{ "(Nacht) Round 94",694 },
		{ "(Nacht) Round 95",695 },
		{ "(Nacht) Round 96",696 },
		{ "(Nacht) Round 97",697 },
		{ "(Nacht) Round 98",698 },
		{ "(Nacht) Round 99",699 },
		{ "(Nacht) Round 100",700 },
		{ "(Kino) Round 01",701 },
		{ "(Kino) Round 02",702 },
		{ "(Kino) Round 03",703 },
		{ "(Kino) Round 04",704 },
		{ "(Kino) Round 05",705 },
		{ "(Kino) Round 06",706 },
		{ "(Kino) Round 07",707 },
		{ "(Kino) Round 08",708 },
		{ "(Kino) Round 09",709 },
		{ "(Kino) Round 10",710 },
		{ "(Kino) Round 11",711 },
		{ "(Kino) Round 12",712 },
		{ "(Kino) Round 13",713 },
		{ "(Kino) Round 14",714 },
		{ "(Kino) Round 15",715 },
		{ "(Kino) Round 16",716 },
		{ "(Kino) Round 17",717 },
		{ "(Kino) Round 18",718 },
		{ "(Kino) Round 19",719 },
		{ "(Kino) Round 20",720 },
		{ "(Kino) Round 21",721 },
		{ "(Kino) Round 22",722 },
		{ "(Kino) Round 23",723 },
		{ "(Kino) Round 24",724 },
		{ "(Kino) Round 25",725 },
		{ "(Kino) Round 26",726 },
		{ "(Kino) Round 27",727 },
		{ "(Kino) Round 28",728 },
		{ "(Kino) Round 29",729 },
		{ "(Kino) Round 30",730 },
		{ "(Kino) Round 31",731 },
		{ "(Kino) Round 32",732 },
		{ "(Kino) Round 33",733 },
		{ "(Kino) Round 34",734 },
		{ "(Kino) Round 35",735 },
		{ "(Kino) Round 36",736 },
		{ "(Kino) Round 37",737 },
		{ "(Kino) Round 38",738 },
		{ "(Kino) Round 39",739 },
		{ "(Kino) Round 40",740 },
		{ "(Kino) Round 41",741 },
		{ "(Kino) Round 42",742 },
		{ "(Kino) Round 43",743 },
		{ "(Kino) Round 44",744 },
		{ "(Kino) Round 45",745 },
		{ "(Kino) Round 46",746 },
		{ "(Kino) Round 47",747 },
		{ "(Kino) Round 48",748 },
		{ "(Kino) Round 49",749 },
		{ "(Kino) Round 50",750 },
		{ "(Kino) Round 51",751 },
		{ "(Kino) Round 52",752 },
		{ "(Kino) Round 53",753 },
		{ "(Kino) Round 54",754 },
		{ "(Kino) Round 55",755 },
		{ "(Kino) Round 56",756 },
		{ "(Kino) Round 57",757 },
		{ "(Kino) Round 58",758 },
		{ "(Kino) Round 59",759 },
		{ "(Kino) Round 60",760 },
		{ "(Kino) Round 61",761 },
		{ "(Kino) Round 62",762 },
		{ "(Kino) Round 63",763 },
		{ "(Kino) Round 64",764 },
		{ "(Kino) Round 65",765 },
		{ "(Kino) Round 66",766 },
		{ "(Kino) Round 67",767 },
		{ "(Kino) Round 68",768 },
		{ "(Kino) Round 69",769 },
		{ "(Kino) Round 70",770 },
		{ "(Kino) Round 71",771 },
		{ "(Kino) Round 72",772 },
		{ "(Kino) Round 73",773 },
		{ "(Kino) Round 74",774 },
		{ "(Kino) Round 75",775 },
		{ "(Kino) Round 76",776 },
		{ "(Kino) Round 77",777 },
		{ "(Kino) Round 78",778 },
		{ "(Kino) Round 79",779 },
		{ "(Kino) Round 80",780 },
		{ "(Kino) Round 81",781 },
		{ "(Kino) Round 82",782 },
		{ "(Kino) Round 83",783 },
		{ "(Kino) Round 84",784 },
		{ "(Kino) Round 85",785 },
		{ "(Kino) Round 86",786 },
		{ "(Kino) Round 87",787 },
		{ "(Kino) Round 88",788 },
		{ "(Kino) Round 89",789 },
		{ "(Kino) Round 90",790 },
		{ "(Kino) Round 91",791 },
		{ "(Kino) Round 92",792 },
		{ "(Kino) Round 93",793 },
		{ "(Kino) Round 94",794 },
		{ "(Kino) Round 95",795 },
		{ "(Kino) Round 96",796 },
		{ "(Kino) Round 97",797 },
		{ "(Kino) Round 98",798 },
		{ "(Kino) Round 99",799 },
		{ "(Kino) Round 100",800 },
		{ "(Shangri-La) Round 01",801 },
		{ "(Shangri-La) Round 02",802 },
		{ "(Shangri-La) Round 03",803 },
		{ "(Shangri-La) Round 04",804 },
		{ "(Shangri-La) Round 05",805 },
		{ "(Shangri-La) Round 06",806 },
		{ "(Shangri-La) Round 07",807 },
		{ "(Shangri-La) Round 08",808 },
		{ "(Shangri-La) Round 09",809 },
		{ "(Shangri-La) Round 10",810 },
		{ "(Shangri-La) Round 11",811 },
		{ "(Shangri-La) Round 12",812 },
		{ "(Shangri-La) Round 13",813 },
		{ "(Shangri-La) Round 14",814 },
		{ "(Shangri-La) Round 15",815 },
		{ "(Shangri-La) Round 16",816 },
		{ "(Shangri-La) Round 17",817 },
		{ "(Shangri-La) Round 18",818 },
		{ "(Shangri-La) Round 19",819 },
		{ "(Shangri-La) Round 20",820 },
		{ "(Shangri-La) Round 21",821 },
		{ "(Shangri-La) Round 22",822 },
		{ "(Shangri-La) Round 23",823 },
		{ "(Shangri-La) Round 24",824 },
		{ "(Shangri-La) Round 25",825 },
		{ "(Shangri-La) Round 26",826 },
		{ "(Shangri-La) Round 27",827 },
		{ "(Shangri-La) Round 28",828 },
		{ "(Shangri-La) Round 29",829 },
		{ "(Shangri-La) Round 30",830 },
		{ "(Shangri-La) Round 31",831 },
		{ "(Shangri-La) Round 32",832 },
		{ "(Shangri-La) Round 33",833 },
		{ "(Shangri-La) Round 34",834 },
		{ "(Shangri-La) Round 35",835 },
		{ "(Shangri-La) Round 36",836 },
		{ "(Shangri-La) Round 37",837 },
		{ "(Shangri-La) Round 38",838 },
		{ "(Shangri-La) Round 39",839 },
		{ "(Shangri-La) Round 40",840 },
		{ "(Shangri-La) Round 41",841 },
		{ "(Shangri-La) Round 42",842 },
		{ "(Shangri-La) Round 43",843 },
		{ "(Shangri-La) Round 44",844 },
		{ "(Shangri-La) Round 45",845 },
		{ "(Shangri-La) Round 46",846 },
		{ "(Shangri-La) Round 47",847 },
		{ "(Shangri-La) Round 48",848 },
		{ "(Shangri-La) Round 49",849 },
		{ "(Shangri-La) Round 50",850 },
		{ "(Shangri-La) Round 51",851 },
		{ "(Shangri-La) Round 52",852 },
		{ "(Shangri-La) Round 53",853 },
		{ "(Shangri-La) Round 54",854 },
		{ "(Shangri-La) Round 55",855 },
		{ "(Shangri-La) Round 56",856 },
		{ "(Shangri-La) Round 57",857 },
		{ "(Shangri-La) Round 58",858 },
		{ "(Shangri-La) Round 59",859 },
		{ "(Shangri-La) Round 60",860 },
		{ "(Shangri-La) Round 61",861 },
		{ "(Shangri-La) Round 62",862 },
		{ "(Shangri-La) Round 63",863 },
		{ "(Shangri-La) Round 64",864 },
		{ "(Shangri-La) Round 65",865 },
		{ "(Shangri-La) Round 66",866 },
		{ "(Shangri-La) Round 67",867 },
		{ "(Shangri-La) Round 68",868 },
		{ "(Shangri-La) Round 69",869 },
		{ "(Shangri-La) Round 70",870 },
		{ "(Shangri-La) Round 71",871 },
		{ "(Shangri-La) Round 72",872 },
		{ "(Shangri-La) Round 73",873 },
		{ "(Shangri-La) Round 74",874 },
		{ "(Shangri-La) Round 75",875 },
		{ "(Shangri-La) Round 76",876 },
		{ "(Shangri-La) Round 77",877 },
		{ "(Shangri-La) Round 78",878 },
		{ "(Shangri-La) Round 79",879 },
		{ "(Shangri-La) Round 80",880 },
		{ "(Shangri-La) Round 81",881 },
		{ "(Shangri-La) Round 82",882 },
		{ "(Shangri-La) Round 83",883 },
		{ "(Shangri-La) Round 84",884 },
		{ "(Shangri-La) Round 85",885 },
		{ "(Shangri-La) Round 86",886 },
		{ "(Shangri-La) Round 87",887 },
		{ "(Shangri-La) Round 88",888 },
		{ "(Shangri-La) Round 89",889 },
		{ "(Shangri-La) Round 90",890 },
		{ "(Shangri-La) Round 91",891 },
		{ "(Shangri-La) Round 92",892 },
		{ "(Shangri-La) Round 93",893 },
		{ "(Shangri-La) Round 94",894 },
		{ "(Shangri-La) Round 95",895 },
		{ "(Shangri-La) Round 96",896 },
		{ "(Shangri-La) Round 97",897 },
		{ "(Shangri-La) Round 98",898 },
		{ "(Shangri-La) Round 99",899 },
		{ "(Shangri-La) Round 100",900 },
		{ "(Origins) Round 01",901 },
		{ "(Origins) Round 02",902 },
		{ "(Origins) Round 03",903 },
		{ "(Origins) Round 04",904 },
		{ "(Origins) Round 05",905 },
		{ "(Origins) Round 06",906 },
		{ "(Origins) Round 07",907 },
		{ "(Origins) Round 08",908 },
		{ "(Origins) Round 09",909 },
		{ "(Origins) Round 10",910 },
		{ "(Origins) Round 11",911 },
		{ "(Origins) Round 12",912 },
		{ "(Origins) Round 13",913 },
		{ "(Origins) Round 14",914 },
		{ "(Origins) Round 15",915 },
		{ "(Origins) Round 16",916 },
		{ "(Origins) Round 17",917 },
		{ "(Origins) Round 18",918 },
		{ "(Origins) Round 19",919 },
		{ "(Origins) Round 20",920 },
		{ "(Origins) Round 21",921 },
		{ "(Origins) Round 22",922 },
		{ "(Origins) Round 23",923 },
		{ "(Origins) Round 24",924 },
		{ "(Origins) Round 25",925 },
		{ "(Origins) Round 26",926 },
		{ "(Origins) Round 27",927 },
		{ "(Origins) Round 28",928 },
		{ "(Origins) Round 29",929 },
		{ "(Origins) Round 30",930 },
		{ "(Origins) Round 31",931 },
		{ "(Origins) Round 32",932 },
		{ "(Origins) Round 33",933 },
		{ "(Origins) Round 34",934 },
		{ "(Origins) Round 35",935 },
		{ "(Origins) Round 36",936 },
		{ "(Origins) Round 37",937 },
		{ "(Origins) Round 38",938 },
		{ "(Origins) Round 39",939 },
		{ "(Origins) Round 40",940 },
		{ "(Origins) Round 41",941 },
		{ "(Origins) Round 42",942 },
		{ "(Origins) Round 43",943 },
		{ "(Origins) Round 44",944 },
		{ "(Origins) Round 45",945 },
		{ "(Origins) Round 46",946 },
		{ "(Origins) Round 47",947 },
		{ "(Origins) Round 48",948 },
		{ "(Origins) Round 49",949 },
		{ "(Origins) Round 50",950 },
		{ "(Origins) Round 51",951 },
		{ "(Origins) Round 52",952 },
		{ "(Origins) Round 53",953 },
		{ "(Origins) Round 54",954 },
		{ "(Origins) Round 55",955 },
		{ "(Origins) Round 56",956 },
		{ "(Origins) Round 57",957 },
		{ "(Origins) Round 58",958 },
		{ "(Origins) Round 59",959 },
		{ "(Origins) Round 60",960 },
		{ "(Origins) Round 61",961 },
		{ "(Origins) Round 62",962 },
		{ "(Origins) Round 63",963 },
		{ "(Origins) Round 64",964 },
		{ "(Origins) Round 65",965 },
		{ "(Origins) Round 66",966 },
		{ "(Origins) Round 67",967 },
		{ "(Origins) Round 68",968 },
		{ "(Origins) Round 69",969 },
		{ "(Origins) Round 70",970 },
		{ "(Origins) Round 71",971 },
		{ "(Origins) Round 72",972 },
		{ "(Origins) Round 73",973 },
		{ "(Origins) Round 74",974 },
		{ "(Origins) Round 75",975 },
		{ "(Origins) Round 76",976 },
		{ "(Origins) Round 77",977 },
		{ "(Origins) Round 78",978 },
		{ "(Origins) Round 79",979 },
		{ "(Origins) Round 80",980 },
		{ "(Origins) Round 81",981 },
		{ "(Origins) Round 82",982 },
		{ "(Origins) Round 83",983 },
		{ "(Origins) Round 84",984 },
		{ "(Origins) Round 85",985 },
		{ "(Origins) Round 86",986 },
		{ "(Origins) Round 87",987 },
		{ "(Origins) Round 88",988 },
		{ "(Origins) Round 89",989 },
		{ "(Origins) Round 90",990 },
		{ "(Origins) Round 91",991 },
		{ "(Origins) Round 92",992 },
		{ "(Origins) Round 93",993 },
		{ "(Origins) Round 94",994 },
		{ "(Origins) Round 95",995 },
		{ "(Origins) Round 96",996 },
		{ "(Origins) Round 97",997 },
		{ "(Origins) Round 98",998 },
		{ "(Origins) Round 99",999 },
		{ "(Origins) Round 100",1000 },
		{ "(Verruckt) Round 01",1001 },
		{ "(Verruckt) Round 02",1002 },
		{ "(Verruckt) Round 03",1003 },
		{ "(Verruckt) Round 04",1004 },
		{ "(Verruckt) Round 05",1005 },
		{ "(Verruckt) Round 06",1006 },
		{ "(Verruckt) Round 07",1007 },
		{ "(Verruckt) Round 08",1008 },
		{ "(Verruckt) Round 09",1009 },
		{ "(Verruckt) Round 10",1010 },
		{ "(Verruckt) Round 11",1011 },
		{ "(Verruckt) Round 12",1012 },
		{ "(Verruckt) Round 13",1013 },
		{ "(Verruckt) Round 14",1014 },
		{ "(Verruckt) Round 15",1015 },
		{ "(Verruckt) Round 16",1016 },
		{ "(Verruckt) Round 17",1017 },
		{ "(Verruckt) Round 18",1018 },
		{ "(Verruckt) Round 19",1019 },
		{ "(Verruckt) Round 20",1020 },
		{ "(Verruckt) Round 21",1021 },
		{ "(Verruckt) Round 22",1022 },
		{ "(Verruckt) Round 23",1023 },
		{ "(Verruckt) Round 24",1024 },
		{ "(Verruckt) Round 25",1025 },
		{ "(Verruckt) Round 26",1026 },
		{ "(Verruckt) Round 27",1027 },
		{ "(Verruckt) Round 28",1028 },
		{ "(Verruckt) Round 29",1029 },
		{ "(Verruckt) Round 30",1030 },
		{ "(Verruckt) Round 31",1031 },
		{ "(Verruckt) Round 32",1032 },
		{ "(Verruckt) Round 33",1033 },
		{ "(Verruckt) Round 34",1034 },
		{ "(Verruckt) Round 35",1035 },
		{ "(Verruckt) Round 36",1036 },
		{ "(Verruckt) Round 37",1037 },
		{ "(Verruckt) Round 38",1038 },
		{ "(Verruckt) Round 39",1039 },
		{ "(Verruckt) Round 40",1040 },
		{ "(Verruckt) Round 41",1041 },
		{ "(Verruckt) Round 42",1042 },
		{ "(Verruckt) Round 43",1043 },
		{ "(Verruckt) Round 44",1044 },
		{ "(Verruckt) Round 45",1045 },
		{ "(Verruckt) Round 46",1046 },
		{ "(Verruckt) Round 47",1047 },
		{ "(Verruckt) Round 48",1048 },
		{ "(Verruckt) Round 49",1049 },
		{ "(Verruckt) Round 50",1050 },
		{ "(Verruckt) Round 51",1051 },
		{ "(Verruckt) Round 52",1052 },
		{ "(Verruckt) Round 53",1053 },
		{ "(Verruckt) Round 54",1054 },
		{ "(Verruckt) Round 55",1055 },
		{ "(Verruckt) Round 56",1056 },
		{ "(Verruckt) Round 57",1057 },
		{ "(Verruckt) Round 58",1058 },
		{ "(Verruckt) Round 59",1059 },
		{ "(Verruckt) Round 60",1060 },
		{ "(Verruckt) Round 61",1061 },
		{ "(Verruckt) Round 62",1062 },
		{ "(Verruckt) Round 63",1063 },
		{ "(Verruckt) Round 64",1064 },
		{ "(Verruckt) Round 65",1065 },
		{ "(Verruckt) Round 66",1066 },
		{ "(Verruckt) Round 67",1067 },
		{ "(Verruckt) Round 68",1068 },
		{ "(Verruckt) Round 69",1069 },
		{ "(Verruckt) Round 70",1070 },
		{ "(Verruckt) Round 71",1071 },
		{ "(Verruckt) Round 72",1072 },
		{ "(Verruckt) Round 73",1073 },
		{ "(Verruckt) Round 74",1074 },
		{ "(Verruckt) Round 75",1075 },
		{ "(Verruckt) Round 76",1076 },
		{ "(Verruckt) Round 77",1077 },
		{ "(Verruckt) Round 78",1078 },
		{ "(Verruckt) Round 79",1079 },
		{ "(Verruckt) Round 80",1080 },
		{ "(Verruckt) Round 81",1081 },
		{ "(Verruckt) Round 82",1082 },
		{ "(Verruckt) Round 83",1083 },
		{ "(Verruckt) Round 84",1084 },
		{ "(Verruckt) Round 85",1085 },
		{ "(Verruckt) Round 86",1086 },
		{ "(Verruckt) Round 87",1087 },
		{ "(Verruckt) Round 88",1088 },
		{ "(Verruckt) Round 89",1089 },
		{ "(Verruckt) Round 90",1090 },
		{ "(Verruckt) Round 91",1091 },
		{ "(Verruckt) Round 92",1092 },
		{ "(Verruckt) Round 93",1093 },
		{ "(Verruckt) Round 94",1094 },
		{ "(Verruckt) Round 95",1095 },
		{ "(Verruckt) Round 96",1096 },
		{ "(Verruckt) Round 97",1097 },
		{ "(Verruckt) Round 98",1098 },
		{ "(Verruckt) Round 99",1099 },
		{ "(Verruckt) Round 100",1100 },
		{ "(Shi no Numa) Round 01",1101 },
		{ "(Shi no Numa) Round 02",1102 },
		{ "(Shi no Numa) Round 03",1103 },
		{ "(Shi no Numa) Round 04",1104 },
		{ "(Shi no Numa) Round 05",1105 },
		{ "(Shi no Numa) Round 06",1106 },
		{ "(Shi no Numa) Round 07",1107 },
		{ "(Shi no Numa) Round 08",1108 },
		{ "(Shi no Numa) Round 09",1109 },
		{ "(Shi no Numa) Round 10",1110 },
		{ "(Shi no Numa) Round 11",1111 },
		{ "(Shi no Numa) Round 12",1112 },
		{ "(Shi no Numa) Round 13",1113 },
		{ "(Shi no Numa) Round 14",1114 },
		{ "(Shi no Numa) Round 15",1115 },
		{ "(Shi no Numa) Round 16",1116 },
		{ "(Shi no Numa) Round 17",1117 },
		{ "(Shi no Numa) Round 18",1118 },
		{ "(Shi no Numa) Round 19",1119 },
		{ "(Shi no Numa) Round 20",1120 },
		{ "(Shi no Numa) Round 21",1121 },
		{ "(Shi no Numa) Round 22",1122 },
		{ "(Shi no Numa) Round 23",1123 },
		{ "(Shi no Numa) Round 24",1124 },
		{ "(Shi no Numa) Round 25",1125 },
		{ "(Shi no Numa) Round 26",1126 },
		{ "(Shi no Numa) Round 27",1127 },
		{ "(Shi no Numa) Round 28",1128 },
		{ "(Shi no Numa) Round 29",1129 },
		{ "(Shi no Numa) Round 30",1130 },
		{ "(Shi no Numa) Round 31",1131 },
		{ "(Shi no Numa) Round 32",1132 },
		{ "(Shi no Numa) Round 33",1133 },
		{ "(Shi no Numa) Round 34",1134 },
		{ "(Shi no Numa) Round 35",1135 },
		{ "(Shi no Numa) Round 36",1136 },
		{ "(Shi no Numa) Round 37",1137 },
		{ "(Shi no Numa) Round 38",1138 },
		{ "(Shi no Numa) Round 39",1139 },
		{ "(Shi no Numa) Round 40",1140 },
		{ "(Shi no Numa) Round 41",1141 },
		{ "(Shi no Numa) Round 42",1142 },
		{ "(Shi no Numa) Round 43",1143 },
		{ "(Shi no Numa) Round 44",1144 },
		{ "(Shi no Numa) Round 45",1145 },
		{ "(Shi no Numa) Round 46",1146 },
		{ "(Shi no Numa) Round 47",1147 },
		{ "(Shi no Numa) Round 48",1148 },
		{ "(Shi no Numa) Round 49",1149 },
		{ "(Shi no Numa) Round 50",1150 },
		{ "(Shi no Numa) Round 51",1151 },
		{ "(Shi no Numa) Round 52",1152 },
		{ "(Shi no Numa) Round 53",1153 },
		{ "(Shi no Numa) Round 54",1154 },
		{ "(Shi no Numa) Round 55",1155 },
		{ "(Shi no Numa) Round 56",1156 },
		{ "(Shi no Numa) Round 57",1157 },
		{ "(Shi no Numa) Round 58",1158 },
		{ "(Shi no Numa) Round 59",1159 },
		{ "(Shi no Numa) Round 60",1160 },
		{ "(Shi no Numa) Round 61",1161 },
		{ "(Shi no Numa) Round 62",1162 },
		{ "(Shi no Numa) Round 63",1163 },
		{ "(Shi no Numa) Round 64",1164 },
		{ "(Shi no Numa) Round 65",1165 },
		{ "(Shi no Numa) Round 66",1166 },
		{ "(Shi no Numa) Round 67",1167 },
		{ "(Shi no Numa) Round 68",1168 },
		{ "(Shi no Numa) Round 69",1169 },
		{ "(Shi no Numa) Round 70",1170 },
		{ "(Shi no Numa) Round 71",1171 },
		{ "(Shi no Numa) Round 72",1172 },
		{ "(Shi no Numa) Round 73",1173 },
		{ "(Shi no Numa) Round 74",1174 },
		{ "(Shi no Numa) Round 75",1175 },
		{ "(Shi no Numa) Round 76",1176 },
		{ "(Shi no Numa) Round 77",1177 },
		{ "(Shi no Numa) Round 78",1178 },
		{ "(Shi no Numa) Round 79",1179 },
		{ "(Shi no Numa) Round 80",1180 },
		{ "(Shi no Numa) Round 81",1181 },
		{ "(Shi no Numa) Round 82",1182 },
		{ "(Shi no Numa) Round 83",1183 },
		{ "(Shi no Numa) Round 84",1184 },
		{ "(Shi no Numa) Round 85",1185 },
		{ "(Shi no Numa) Round 86",1186 },
		{ "(Shi no Numa) Round 87",1187 },
		{ "(Shi no Numa) Round 88",1188 },
		{ "(Shi no Numa) Round 89",1189 },
		{ "(Shi no Numa) Round 90",1190 },
		{ "(Shi no Numa) Round 91",1191 },
		{ "(Shi no Numa) Round 92",1192 },
		{ "(Shi no Numa) Round 93",1193 },
		{ "(Shi no Numa) Round 94",1194 },
		{ "(Shi no Numa) Round 95",1195 },
		{ "(Shi no Numa) Round 96",1196 },
		{ "(Shi no Numa) Round 97",1197 },
		{ "(Shi no Numa) Round 98",1198 },
		{ "(Shi no Numa) Round 99",1199 },
		{ "(Shi no Numa) Round 100",1200 },
		{ "(Ascension) Round 01",1201 },
		{ "(Ascension) Round 02",1202 },
		{ "(Ascension) Round 03",1203 },
		{ "(Ascension) Round 04",1204 },
		{ "(Ascension) Round 05",1205 },
		{ "(Ascension) Round 06",1206 },
		{ "(Ascension) Round 07",1207 },
		{ "(Ascension) Round 08",1208 },
		{ "(Ascension) Round 09",1209 },
		{ "(Ascension) Round 10",1210 },
		{ "(Ascension) Round 11",1211 },
		{ "(Ascension) Round 12",1212 },
		{ "(Ascension) Round 13",1213 },
		{ "(Ascension) Round 14",1214 },
		{ "(Ascension) Round 15",1215 },
		{ "(Ascension) Round 16",1216 },
		{ "(Ascension) Round 17",1217 },
		{ "(Ascension) Round 18",1218 },
		{ "(Ascension) Round 19",1219 },
		{ "(Ascension) Round 20",1220 },
		{ "(Ascension) Round 21",1221 },
		{ "(Ascension) Round 22",1222 },
		{ "(Ascension) Round 23",1223 },
		{ "(Ascension) Round 24",1224 },
		{ "(Ascension) Round 25",1225 },
		{ "(Ascension) Round 26",1226 },
		{ "(Ascension) Round 27",1227 },
		{ "(Ascension) Round 28",1228 },
		{ "(Ascension) Round 29",1229 },
		{ "(Ascension) Round 30",1230 },
		{ "(Ascension) Round 31",1231 },
		{ "(Ascension) Round 32",1232 },
		{ "(Ascension) Round 33",1233 },
		{ "(Ascension) Round 34",1234 },
		{ "(Ascension) Round 35",1235 },
		{ "(Ascension) Round 36",1236 },
		{ "(Ascension) Round 37",1237 },
		{ "(Ascension) Round 38",1238 },
		{ "(Ascension) Round 39",1239 },
		{ "(Ascension) Round 40",1240 },
		{ "(Ascension) Round 41",1241 },
		{ "(Ascension) Round 42",1242 },
		{ "(Ascension) Round 43",1243 },
		{ "(Ascension) Round 44",1244 },
		{ "(Ascension) Round 45",1245 },
		{ "(Ascension) Round 46",1246 },
		{ "(Ascension) Round 47",1247 },
		{ "(Ascension) Round 48",1248 },
		{ "(Ascension) Round 49",1249 },
		{ "(Ascension) Round 50",1250 },
		{ "(Ascension) Round 51",1251 },
		{ "(Ascension) Round 52",1252 },
		{ "(Ascension) Round 53",1253 },
		{ "(Ascension) Round 54",1254 },
		{ "(Ascension) Round 55",1255 },
		{ "(Ascension) Round 56",1256 },
		{ "(Ascension) Round 57",1257 },
		{ "(Ascension) Round 58",1258 },
		{ "(Ascension) Round 59",1259 },
		{ "(Ascension) Round 60",1260 },
		{ "(Ascension) Round 61",1261 },
		{ "(Ascension) Round 62",1262 },
		{ "(Ascension) Round 63",1263 },
		{ "(Ascension) Round 64",1264 },
		{ "(Ascension) Round 65",1265 },
		{ "(Ascension) Round 66",1266 },
		{ "(Ascension) Round 67",1267 },
		{ "(Ascension) Round 68",1268 },
		{ "(Ascension) Round 69",1269 },
		{ "(Ascension) Round 70",1270 },
		{ "(Ascension) Round 71",1271 },
		{ "(Ascension) Round 72",1272 },
		{ "(Ascension) Round 73",1273 },
		{ "(Ascension) Round 74",1274 },
		{ "(Ascension) Round 75",1275 },
		{ "(Ascension) Round 76",1276 },
		{ "(Ascension) Round 77",1277 },
		{ "(Ascension) Round 78",1278 },
		{ "(Ascension) Round 79",1279 },
		{ "(Ascension) Round 80",1280 },
		{ "(Ascension) Round 81",1281 },
		{ "(Ascension) Round 82",1282 },
		{ "(Ascension) Round 83",1283 },
		{ "(Ascension) Round 84",1284 },
		{ "(Ascension) Round 85",1285 },
		{ "(Ascension) Round 86",1286 },
		{ "(Ascension) Round 87",1287 },
		{ "(Ascension) Round 88",1288 },
		{ "(Ascension) Round 89",1289 },
		{ "(Ascension) Round 90",1290 },
		{ "(Ascension) Round 91",1291 },
		{ "(Ascension) Round 92",1292 },
		{ "(Ascension) Round 93",1293 },
		{ "(Ascension) Round 94",1294 },
		{ "(Ascension) Round 95",1295 },
		{ "(Ascension) Round 96",1296 },
		{ "(Ascension) Round 97",1297 },
		{ "(Ascension) Round 98",1298 },
		{ "(Ascension) Round 99",1299 },
		{ "(Ascension) Round 100",1200 },
		{ "(Moon) Round 01",1301 },
		{ "(Moon) Round 02",1302 },
		{ "(Moon) Round 03",1303 },
		{ "(Moon) Round 04",1304 },
		{ "(Moon) Round 05",1305 },
		{ "(Moon) Round 06",1306 },
		{ "(Moon) Round 07",1307 },
		{ "(Moon) Round 08",1308 },
		{ "(Moon) Round 09",1309 },
		{ "(Moon) Round 10",1310 },
		{ "(Moon) Round 11",1311 },
		{ "(Moon) Round 12",1312 },
		{ "(Moon) Round 13",1313 },
		{ "(Moon) Round 14",1314 },
		{ "(Moon) Round 15",1315 },
		{ "(Moon) Round 16",1316 },
		{ "(Moon) Round 17",1317 },
		{ "(Moon) Round 18",1318 },
		{ "(Moon) Round 19",1319 },
		{ "(Moon) Round 20",1320 },
		{ "(Moon) Round 21",1321 },
		{ "(Moon) Round 22",1322 },
		{ "(Moon) Round 23",1323 },
		{ "(Moon) Round 24",1324 },
		{ "(Moon) Round 25",1325 },
		{ "(Moon) Round 26",1326 },
		{ "(Moon) Round 27",1327 },
		{ "(Moon) Round 28",1328 },
		{ "(Moon) Round 29",1329 },
		{ "(Moon) Round 30",1330 },
		{ "(Moon) Round 31",1331 },
		{ "(Moon) Round 32",1332 },
		{ "(Moon) Round 33",1333 },
		{ "(Moon) Round 34",1334 },
		{ "(Moon) Round 35",1335 },
		{ "(Moon) Round 36",1336 },
		{ "(Moon) Round 37",1337 },
		{ "(Moon) Round 38",1338 },
		{ "(Moon) Round 39",1339 },
		{ "(Moon) Round 40",1340 },
		{ "(Moon) Round 41",1341 },
		{ "(Moon) Round 42",1342 },
		{ "(Moon) Round 43",1343 },
		{ "(Moon) Round 44",1344 },
		{ "(Moon) Round 45",1345 },
		{ "(Moon) Round 46",1346 },
		{ "(Moon) Round 47",1347 },
		{ "(Moon) Round 48",1348 },
		{ "(Moon) Round 49",1349 },
		{ "(Moon) Round 50",1350 },
		{ "(Moon) Round 51",1351 },
		{ "(Moon) Round 52",1352 },
		{ "(Moon) Round 53",1353 },
		{ "(Moon) Round 54",1354 },
		{ "(Moon) Round 55",1355 },
		{ "(Moon) Round 56",1356 },
		{ "(Moon) Round 57",1357 },
		{ "(Moon) Round 58",1358 },
		{ "(Moon) Round 59",1359 },
		{ "(Moon) Round 60",1360 },
		{ "(Moon) Round 61",1361 },
		{ "(Moon) Round 62",1362 },
		{ "(Moon) Round 63",1363 },
		{ "(Moon) Round 64",1364 },
		{ "(Moon) Round 65",1365 },
		{ "(Moon) Round 66",1366 },
		{ "(Moon) Round 67",1367 },
		{ "(Moon) Round 68",1368 },
		{ "(Moon) Round 69",1369 },
		{ "(Moon) Round 70",1370 },
		{ "(Moon) Round 71",1371 },
		{ "(Moon) Round 72",1372 },
		{ "(Moon) Round 73",1373 },
		{ "(Moon) Round 74",1374 },
		{ "(Moon) Round 75",1375 },
		{ "(Moon) Round 76",1376 },
		{ "(Moon) Round 77",1377 },
		{ "(Moon) Round 78",1378 },
		{ "(Moon) Round 79",1379 },
		{ "(Moon) Round 80",1380 },
		{ "(Moon) Round 81",1381 },
		{ "(Moon) Round 82",1382 },
		{ "(Moon) Round 83",1383 },
		{ "(Moon) Round 84",1384 },
		{ "(Moon) Round 85",1385 },
		{ "(Moon) Round 86",1386 },
		{ "(Moon) Round 87",1387 },
		{ "(Moon) Round 88",1388 },
		{ "(Moon) Round 89",1389 },
		{ "(Moon) Round 90",1390 },
		{ "(Moon) Round 91",1391 },
		{ "(Moon) Round 92",1392 },
		{ "(Moon) Round 93",1393 },
		{ "(Moon) Round 94",1394 },
		{ "(Moon) Round 95",1395 },
		{ "(Moon) Round 96",1396 },
		{ "(Moon) Round 97",1397 },
		{ "(Moon) Round 98",1398 },
		{ "(Moon) Round 99",1399 },
		{ "(Moon) Round 100",1400 },

	};

	std::unordered_map<std::string, std::string> settings{
		{"the_giant_enabled","AP_THE_GIANT_ENABLED"}
	};

	//Utility Functions
	void APLogPrint(std::string message)
	{
		std::string luaThreadCode = "Archi.LogMessage(\"" + message + "\");";
		hks::execute_raw_lua(luaThreadCode, "APLogThread");
	}
	void APDebugLogPrint(std::string message)
	{
		std::string luaThreadCode = "Archi.LogDebugMessage(\"" + message + "\");";
		hks::execute_raw_lua(luaThreadCode, "APLogThread");
	}

	void APSetDvar(std::string var, std::string val)
	{
		std::string luaThreadCode = "Engine.SetDvar( \""+var+"\", \"" + val + "\" )";
		hks::execute_raw_lua(luaThreadCode, "APSetDvarThread");
	}

	//Actual C++ functions
	void disconnect_ap()
	{
		if (ap) delete ap;
		ap = nullptr;
	}

	void check_connection_ap(std::string uri = "", std::string slot = "", std::string uuidFile = ".\\mods\\bo3_archipelago\\zone\\uuid", std::string password = "")
	{

		std::string uuid = ap_get_uuid(UUID_FILE);

		if (temp_ap != nullptr)
		{	
			temp_ap->reset();
		}

		std::string luaThreadCode = "UpdateConnectionStatus(\"Connecting...\")";
		hks::execute_raw_lua(luaThreadCode, "SetConnectionValidatedThread");

		temp_ap = new APClient(uuid, GAME_NAME, uri.empty() ? APClient::DEFAULT_URI : uri);


		temp_ap->set_socket_connected_handler([]() {
			});
		temp_ap->set_socket_disconnected_handler([]() {
			});

		temp_ap->set_socket_error_handler([](const std::string& error) {
				//On Error don't try and reconnect
				std::string luaThreadCode = "UpdateConnectionStatus(\"Socket Error: " + error + " \")";
				hks::execute_raw_lua(luaThreadCode, "SetConnectionValidatedThread");
			});

		temp_ap->set_room_info_handler([slot, password]() {

			std::list<std::string> tags;
			tags.push_back("TextOnly");
			temp_ap->ConnectSlot(slot, password, items_handling, tags, VERSION_TUPLE);
			});
		temp_ap->set_slot_connected_handler([](const json& data) {


			//Mandatory values
			if (!data.contains("base_id") || !data.contains("seed") || !data.contains("slot")) {

			}

			std::string luaThreadCode = "UpdateConnectionStatus(\"Validated\")";
			hks::execute_raw_lua(luaThreadCode, "SetConnectionValidatedThread");

		});
			



		temp_ap->set_slot_refused_handler([](const std::list<std::string>& errors) {
				//On Error don't try and reconnect
				std::string luaThreadCode = "UpdateConnectionStatus(\"RefusedHandler\")";
				if (std::find(errors.begin(), errors.end(), "InvalidSlot") != errors.end()) {
					luaThreadCode = "UpdateConnectionStatus(\"InvalidSlot\")";
				}
				else {
					
				}
				hks::execute_raw_lua(luaThreadCode, "SetConnectionValidatedThread");
				});

		for (int i = 0; i < 10; i++)
		{
			if (temp_ap){
			
				temp_ap->poll();
				Sleep(1000);
			}
		}
		if (temp_ap) 
		{	
			delete temp_ap;
		}
		temp_ap = nullptr;
			
	}


	void connect_ap(std::string uri = "",std::string slot="",std::string uuidFile = ".\\mods\\bo3_archipelago\\zone\\uuid", std::string password = "")
	{
		console::print_info("connect_ap: entered ap client");
		std::string uuid = ap_get_uuid(UUID_FILE);

		if (ap != nullptr) {
			return;
		}
		ap = new APClient(uuid, GAME_NAME, uri.empty() ? APClient::DEFAULT_URI : uri);

		console::print_info("connect_ap: made new client");
		/*
		 TODO: DataPackage Cache
		*/

		ap->set_socket_connected_handler([]() {
			console::print_info("connect_ap: set_socket_connected_handler called");
			socket_connected = true;
			});
		ap->set_socket_disconnected_handler([]() {
			console::print_info("connect_ap: set_socket_disconnected_handler called");
			socket_connected = false;
			archipelago::awaitingReconnect = true;
			APLogPrint("Socket disconnected");
			});


		ap->set_room_info_handler([slot, password]() {
				console::print_info("connect_ap: set_room_info_handler called");
				std::list<std::string> tags;
				ap->ConnectSlot(slot, password, items_handling, tags, VERSION_TUPLE);
			});
		ap->set_slot_connected_handler([](const json& data) {
			console::print_info("connect_ap: set_slot_connected_handler called");
			APLogPrint("Slot connected");
			//Mandatory values
			if (!data.contains("base_id") || !data.contains("seed") || !data.contains("slot")) {
				//TODO Disconnect/End Game or something :/
				console::print_error("Error thingamabob????");
			}

			std::string idStr;
			data.at("base_id").get_to(idStr);


			baseID = std::stoi(idStr);

			data.at("seed").get_to(archipelago::seed);
			data.at("slot").get_to(archipelago::slot);

			/* TODO: Pull settings once i add some
			for (auto& [jsonName, dVar] : settings)
			{
				if (data.contains(jsonName))
				{
					std::string val;
					data.at(jsonName).get_to(val);
					APSetDvar(dVar, val);
				}
			}*/

			});
		ap->set_socket_error_handler([](const std::string& error) {
			//On Error don't try and reconnect
			APLogPrint(error.c_str());
			});
		ap->set_slot_disconnected_handler([]() {
			APLogPrint("Slot Disconnected");
			});
		ap->set_slot_refused_handler([](const std::list<std::string>& errors) {
			if (std::find(errors.begin(), errors.end(), "InvalidSlot") != errors.end()) {
				APLogPrint("Slot Invalid");
			}
			else {
				APLogPrint("Connection refused:");
				for (const auto& error : errors) APLogPrint(error.c_str());
			}
			});
		ap->set_items_received_handler([](const std::list<APClient::NetworkItem>& items) {

			if (!ap->is_data_package_valid()) {
				// NOTE: this should not happen since we ask for data package before connecting
				if (!ap_sync_queued) ap->Sync();
				ap_sync_queued = true;
				return;
			}

			std::list<APClient::NetworkItem> valid_items = std::list<APClient::NetworkItem>(items);
			
			/* TODO: Get this to actually work
			* 
			* //If we are awaiting the big reconnect dump of items, skip to after the last item we know about
			if (archipelago::awaitingReconnect)
			{
				if (items.size() > archipelago::lastItem)
				{
					auto lastIter = valid_items.begin();
					std::advance(lastIter, archipelago::lastItem);
					valid_items.erase(valid_items.begin(), lastIter);
				}

				archipelago::awaitingReconnect = false;
			}*/
			

			for (const auto& item : valid_items) {
				archipelago::lastItem += 1;

				std::string itemname = ap->get_item_name(item.item, "Black Ops 3 - Zombies");
				std::string sender = ap->get_player_alias(item.player);
				std::string location = ap->get_location_name(item.location, "Black Ops 3 - Zombies");

				std::string luaThreadCode = "Archi.ItemGetEvent(\""+itemname+"\");";
				hks::execute_raw_lua(luaThreadCode, "ItemGetThread");
			}
			});

		ap->set_data_package_changed_handler([](const json& data) {
			});
		ap->set_print_handler([](const std::string& msg) {
			APLogPrint(msg);
			});
		ap->set_print_json_handler([](const std::list<APClient::TextNode>& msg) {
			APLogPrint(ap->render_json(msg, APClient::RenderFormat::TEXT).c_str());
			});
		ap->set_location_info_handler([](const std::list<APClient::NetworkItem>& items) {

			console::print_info("Reached Location Info Handling...");
			for (const auto& item : items) {
				APSetDvar("soe_specialty_quickrevive_item",ap->get_player_alias(item.player) + "'s " + ap->get_item_name(item.item, ""));
				APSetDvar("soe_specialty_staminup_item", ap->get_item_name(item.item, "te"));
			}
			});

		ap->set_bounced_handler([](const json& cmd) {
			//TODO Implement Deathlink
			});

	}

	int64_t getIdFromString(std::string name)
	{
		int result;
		try
		{
			result = loc_name_to_id.at(name) - 1;
		}
		catch (std::out_of_range e)
		{
			//Not a valid item recieved
			return archipelago::loc_error;
		}
		return (int64_t)(result+baseID);
	}

	//Lua State Functions 

	int connect(lua::lua_State* s)
	{

		console::print_info("ap connect called");
		std::string url = lua::lua_tostring(s, 1);
		std::string slot = lua::lua_tostring(s, 2);
		std::string uuidPath = lua::lua_tostring(s, 3);

		std::string password = "";

		connect_ap(url, slot,uuidPath + UUID_FILE,password);

		return 1;
	}


	int checkConnection(lua::lua_State* s)
	{

		console::print_info("ap check connect called");

		std::string url = lua::lua_tostring(s, 1);
		std::string slot = lua::lua_tostring(s, 2);
		std::string uuidPath = lua::lua_tostring(s, 3);

		//TODO: Support Password Argument
		std::string password = "";

		check_connection_ap(url, slot, uuidPath + UUID_FILE, password);

		

		return 1;
	}

	int disconnect(lua::lua_State* s)
	{
		console::print_info("disconnect call");

		disconnect_ap();
		return 1;
	}

	int poll(lua::lua_State* s)
	{
		if (ap) ap->poll();

		int size_check = archipelago::checkedLocationsList.size();
		int size_scout = archipelago::scoutedLocationsList.size();
		if (ap && size_check > 0) {
			console::print_info("Location Checking...");
			if (ap->LocationChecks(archipelago::checkedLocationsList)) {
				archipelago::checkedLocationsList.clear();
			}
		}
		if (ap && size_scout > 0) {
			console::print_info("Scout in poll...");
			if (ap->LocationScouts(archipelago::scoutedLocationsList, 2)) {
				archipelago::scoutedLocationsList.clear();
			}
		}
		return 1;
	}

	int checkLocation(lua::lua_State* s)
	{
		std::string loc = lua::lua_tostring(s, 1);


		int64_t loc_id = getIdFromString(loc);
		if (loc_id != archipelago::loc_error)
		{
			checkedLocationsList.push_back(loc_id);
		}
		else
		{
			console::print_error("ERROR getting id");
		}
		return 1;
	}

	int scoutLocation(lua::lua_State* s)
	{
		APLogPrint("Location Scoutting...");
		std::string loc = lua::lua_tostring(s, 1);

		int64_t loc_id = getIdFromString(loc);
		if (loc_id != archipelago::loc_error) {
			scoutedLocationsList.push_back(loc_id);
		}
		else {
			console::print_error("ERROR getting id");
		}
		return 1;
	}

	int socketConnected(lua::lua_State* s)
	{
		lua::lua_pushboolean(s, socket_connected);
		return 1;
	}

	class component final : public component_interface
	{
	public:
		void lua_start() override
		{

			console::print_info("lua_start called");
			const lua::luaL_Reg ArchipelagoLibrary[] =
			{
				{"Connect", connect},
				{"CheckConnection",checkConnection},
				{"Disconnect",disconnect},
				{"Poll",poll},
				{"CheckLocation",checkLocation},
				{"ScoutLocation", scoutLocation},
				{nullptr, nullptr},
			};
			hks::hksI_openlib(game::UI_luaVM, "Archipelago", ArchipelagoLibrary, 0, 1);
		}
	};
}

REGISTER_COMPONENT(archipelago::component)