
#proto_library(
#)

cc_library(
    name = 'common',
    srcs = [
	   '../tinyxml/tinystr.cpp',
	   '../tinyxml/tinyxml.cpp',
	   '../tinyxml/tinyxmlerror.cpp',
	   '../tinyxml/tinyxmlparser.cpp',
	   'svr_base/ApplicationBase.cpp',
	   'svr_base/ApplicationFunc.cpp',
	   'svr_base/ApplicationBaseExt.cpp',
	   'svr_base/server_manager.cpp',
	   'svr_base/router_manager.cpp',
	   'net_helper/net_helper.cpp',
	   'net_helper/router_mgr.cpp',
	   'transaction/transaction_mgr.cpp',
	   'transaction/base_transaction.cpp',
    ],

    deps = [
		'//slience:slience',
		'#pthread',
		'//mariadb-connector-c-master:mysqlclient'
    ],

    #link_all_symbols = True,
)


