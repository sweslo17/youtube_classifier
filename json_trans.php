<?php
$handle = fopen($argv[1],"r");
$output_handle = fopen($argv[2],"w");
$data = json_decode(fread($handle,filesize($argv[1])),true);
$i=0;
foreach($data as $main_class)
{
	//fprintf($output_handle,"\t");
	fprintf($output_handle,"%s\n",$main_class['name']);
	//$i++;
	foreach($main_class['keyword'] as $keyword)
	{	
		fprintf($output_handle,"\t");
		fprintf($output_handle,"%s\n",$keyword['term']);
	}
	//fprintf($output_handle,"\n");
	foreach($main_class['subclass'] as $sub_class)
	{
		fprintf($output_handle,"%s/%s\n",$main_class['name'],$sub_class['name']);
		//$i++;
		foreach($sub_class['keyword'] as $keyword)
		{	
			fprintf($output_handle,"\t");
			fprintf($output_handle,"%s\n",$keyword['term']);
		}
	}
}
fclose($output_handle);
?>
