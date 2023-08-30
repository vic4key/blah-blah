<?php

trait SingletonT
{
  protected static $m_instances = array();

  protected function  __construct()
  {
  }

  final protected function  __clone()
  {
  }

  final public static function instance()
  {
    $called_class = get_called_class();
    if (!isset(static::$m_instances[$called_class]))
    {
      static::$m_instances[$called_class] = new $called_class();
    }

    return static::$m_instances[$called_class];
  }
}

class Service
{
  use SingletonT;
  
  private $m_var = null;
  
  public function f()
  {
    if ($this->m_var == null)
    {
      $this->m_var = 123; echo "f() -> m_var is set.\n";
    }
    
    echo "f() -> called.\n";
  }
}

Service::instance()->f();
Service::instance()->f();
Service::instance()->f();

/*

Output:

f() -> m_var is set.
f() -> called.
f() -> called.
f() -> called.

*/

?>