<?php

/**
 * Request a http/https.
 * @param  string  $method  The method (GET / POST / etc)
 * @param  string  $uri     The URI.
 * @param  string  $body    The body.
 * @param  array   $headers The headers.
 * @param  number  $timeout The time-out in seconds.
 * @return any	The response.
 */
function cURL($method, $uri, $headers = [], $body = [], $timeout = 5)
{
	$curl = curl_init();

	curl_setopt($curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0");
	curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, $timeout);
	curl_setopt($curl, CURLOPT_TIMEOUT, $timeout);

	curl_setopt($curl, CURLOPT_URL, $uri);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($curl, CURLOPT_CUSTOMREQUEST, $method);
	curl_setopt($curl, CURLOPT_HTTPHEADER, $headers);
  curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, FALSE);

	if (strtoupper($method) == "POST")
	{
		curl_setopt($curl, CURLOPT_POSTFIELDS, $body);
	}

	$data = curl_exec($curl);

  // $errnum = curl_errno($curl);
  // $errstr = curl_strerror($errnum);
  // echo "Error: $errnum - $errstr";
  // $status = curl_getinfo($curl, CURLINFO_HTTP_CODE);
  // echo "Status: $status";

	curl_close($curl);

	return $data;
}

?>