#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tempered.h>
#include "rrd.h"

/**
This example shows how to enumerate the attached devices, print some
information about them, and read their sensors once.
*/

/** Get and print the sensor values for a given device and sensor. */
void read_device_sensor( tempered_device *device, int sensor )
{
	printf( "\tSensor %i:\n", sensor );
	int type = tempered_get_sensor_type( device, sensor );
	if ( type == TEMPERED_SENSOR_TYPE_NONE )
	{
		printf( "\t\tNo such sensor, or type is not supported.\n" );
	}
	if ( type & TEMPERED_SENSOR_TYPE_TEMPERATURE )
	{
		float tempC;
		if ( tempered_get_temperature( device, sensor, &tempC ) )
		{
			printf( "\t\tTemperature: %.2f°C\n", tempC );
 			char *argv[3];
			int   argc = 3;
			char *val;
 			
			argv[0] = "dummy";
			argv[1] = "/var/spool/temper/temp.rrd";

			sprintf(argv[2],"N:%f", tempC);
			printf( "updating using '%s'\n",argv[2]);
			// FIXME : use  pthread_mutex_lock ? 
			// http://code.ohloh.net/file?fid=IRE0Wa3peIZWPnOvM5A4r1zASEY&cid=1QHk-YyoAGE&s=rrd_update%28&browser=Default#L33
			rrd_update(argc, argv);

			if (rrd_test_error()) {
				printf("ERROR: %s\n", rrd_get_error());
				rrd_clear_error();
			}

			if(tempC>20){
				printf("No need to warmup\n");
			} else {
				printf("Need to warmup\n");
			}
		}
		else
		{
			printf(
				"\t\tFailed to get the temperature: %s\n",
				tempered_error( device )
			);
		}
	}
}

/** Get and print information and sensor values for a given device. */
void read_device( struct tempered_device_list *dev )
{
	printf(
		"Device %s : USB IDs %04x:%04x, interface %i\n"
			"\tEnumeration type name: %s\n",
		dev->path,
		dev->vendor_id, dev->product_id,
		dev->interface_number,
		dev->type_name
	);
	char *error = NULL;
	tempered_device *device = tempered_open( dev, &error );
	if ( device == NULL )
	{
		printf( "\tOpen failed, error: %s\n", error );
		free( error );
		return;
	}
	printf(
		"\tOpen succeeded.\n"
			"\tDevice path: %s\n"
			"\tDevice type name: %s\n",
		tempered_get_device_path( device ),
		tempered_get_type_name( device )
	);
	if ( !tempered_read_sensors( device ) )
	{
		printf(
			"\tFailed to read the sensors: %s\n",
			tempered_error( device )
		);
	}
	else
	{
		int sensor, sensors = tempered_get_sensor_count( device );
		printf( "\tSensor count: %i\n", sensors );
		for ( sensor = 0; sensor < sensors; sensor++ )
		{
			read_device_sensor( device, sensor );
		}
	}
	tempered_close( device );
	printf("device closed\n");
}

int main( void )
{
	char *error = NULL;
	if ( !tempered_init( &error ) )
	{
		fprintf( stderr, "Failed to initialize libtempered: %s\n", error );
		free( error );
		return 1;
	}
	
	struct tempered_device_list *list = tempered_enumerate( &error );
	if ( list == NULL )
	{
		if ( error == NULL )
		{
			printf( "No devices were found.\n" );
		}
		else
		{
			fprintf( stderr, "Failed to enumerate devices: %s\n", error );
			free( error );
		}
	}
	else
	{
		struct tempered_device_list *dev;
		for ( dev = list ; dev != NULL ; dev = dev->next )
		{
			read_device( dev );
		}
		tempered_free_device_list( list );
	}
	
	if ( !tempered_exit( &error ) )
	{
		fprintf( stderr, "Failed to shut down libtempered: %s\n", error );
		free( error );
		return 1;
	}
	return 0;
}
