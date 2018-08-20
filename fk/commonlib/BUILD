
#proto_library(
#)

cc_library(
    name = 'common',
    srcs = [
	   'svr_base/ApplicationBase.cpp',
	   'svr_base/ApplicationFunc.cpp',
	   'transaction/transaction_mgr.cpp',
    ],

    deps = [
		'//slience:slience',
		'#pthread',
		'//mariadb-connector-c-master:mysqlclient'
    ],

    #link_all_symbols = True,
)


