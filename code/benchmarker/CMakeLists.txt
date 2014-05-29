#Setup directory paths
SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR})
SET(HEADERS ${ROOT}/headers)
SET(SOURCES ${ROOT}/sources)

#Initiate source files
FILE(GLOB VI_BENCHMARKER_SOURCES ${SOURCES}/*.cpp)
FILE(GLOB VI_BENCHMARKER_HEADERS ${HEADERS}/*.h)

#Create library
ADD_LIBRARY(vibenchmarker SHARED ${VI_BENCHMARKER_HEADERS} ${VI_BENCHMARKER_SOURCES})
TARGET_LINK_LIBRARIES(vibenchmarker
	viessentials
	vicrosscorrelator

	vipredictionnoisedetector
	vimadnoisedetector
	vipredictionnoisedetector
	vifouriernoisedetector
	viarmanoisedetector
	vizscorenoisedetector
	vinearestneighbournoisedetector
	vimahalanobisnoisedetector

	vipiecewiseconstantinterpolator
	visplineinterpolator
	vihermiteinterpolator
	vicosineinterpolator
	vifourierinterpolator
	vipolynomialinterpolator
	viarmainterpolator

	vipolynomialpredictor
	vihermitepredictor
	vifourierpredictor
	viarmapredictor
	vigarchpredictor
	viconstantpredictor
	vilagrangepredictor
	vinewtonpredictor
)
QT5_USE_MODULES(vibenchmarker Xml Multimedia)

#Install file
INSTALL(TARGETS vibenchmarker DESTINATION benchmarker)
